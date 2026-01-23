#include "game_object_manager.h"

namespace game {
class GameObjectContainer final {
public:
    void clear() noexcept {
        objects_.clear();
        creation_.clear();
        hit_.clear();
        delete_.clear();

        creation_.shrink_to_fit();
        hit_.shrink_to_fit();
        delete_.shrink_to_fit();
    }

    void registerCreation(std::function<std::unique_ptr<game_object>()> f, const UINT64 h) noexcept {
        creation_.emplace_back(h, std::move(f));
    }

public:
    std::vector<std::pair<UINT64, std::function<std::unique_ptr<game_object>()>>> creation_{};  /// 生成関数
    std::unordered_map<UINT64, std::unique_ptr<game_object>>                      objects_{};   /// ゲームオブジェクト
    std::vector<std::pair<std::unique_ptr<game_object>, int>>                     delete_{};    /// 削除オブジェクトハンドル
    std::vector<UINT64>                                                           hit_{};       /// 衝突判定オブジェクトハンドル
};
GameObjectContainer container_{};

void GameObjectManager::registerCreation(std::function<std::unique_ptr<game_object>()> create, const UINT64 handle) noexcept {
    container_.registerCreation(std::move(create), handle);
}



void GameObjectManager::update() noexcept {
    if (!container_.creation_.empty()) {
        for (auto& create : container_.creation_) {
            auto p = create.second();
            p->setHandle(create.first);
            p->initialize();
            container_.objects_.emplace(create.first, std::move(p));
        }
        container_.creation_.clear();
    }
    for (auto& it : container_.objects_) {
        it.second->update();
    }
}

void GameObjectManager::postUpdate() noexcept {
    if (!container_.hit_.empty()) {
        for (auto handle : container_.hit_) {
            auto obj = gameObject(handle);
            if (!obj) {
                continue;
            }
            auto myPos = obj.value()->world().r[3];

            for (auto& it : container_.objects_) {
                if (it.first == handle ||
                    it.second->typeId() != obj.value()->hitTargetTypeId()) {
                    continue;
                }
                auto hitRadius = obj.value()->radius() + it.second->radius();
                auto targetPos = it.second->world().r[3];
                auto distance  = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(myPos, targetPos)));
                if (distance < hitRadius) {
                    obj.value()->onHit();
                    it.second->onHit();
                }
            }
        }
        container_.hit_.clear();
    }
    if (!container_.delete_.empty()) {
        for (auto d = container_.delete_.begin(); d != container_.delete_.end();) {
            if ((d->second--) > 0) {
                ++d;
                continue;
            }
            d = container_.delete_.erase(d);
        }
    }
}
void GameObjectManager::draw(const command_list& commandList) noexcept {
    constexpr UINT objectShaderSlot_ = 1;
    for (auto& it : container_.objects_) {
        it.second->updateDrawBuffer();
        it.second->setDrawCommand(commandList, objectShaderSlot_);
    }
}

void GameObjectManager::clear() noexcept {
    container_.clear();
}

[[nodiscard]] std::optional<game_object*> GameObjectManager::gameObject(UINT64 handle) noexcept {
    auto it = container_.objects_.find(handle);
    if (it == container_.objects_.end()) {
        auto find = std::find_if(container_.creation_.begin(), container_.creation_.end(),
                                 [handle](const auto& pair) { return pair.first == handle; });

        if (find == container_.creation_.end()) {
            return std::nullopt;
        }
    }

    return it->second.get();
}

void GameObjectManager::registerDelete(UINT64 handle) noexcept {
    auto it = container_.objects_.find(handle);
    if (it == container_.objects_.end()) {
        return;
    }

    container_.delete_.emplace_back(std::move(it->second), 10);
    container_.objects_.erase(it);
}

void GameObjectManager::registerHit(UINT64 handle) noexcept {
    container_.hit_.emplace_back(handle);
}

GameObjectManager::~GameObjectManager() {
    clear();
}

}  // namespace game
