#include "descriptor_heap.h"
#include "device.h"
#include <cassert>
#include <wrl/client.h>

class descriptor_heap final {
public:
    descriptor_heap() = default;

    ~descriptor_heap() = default;

public:
    [[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
        heapDesc.Type           = type;
        heapDesc.NumDescriptors = numDescriptors;
        heapDesc.Flags          = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        type_ = type;

        HRESULT hr = device::instance().get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
        if (FAILED(hr)) {
            assert(false && "ディスクリプタヒープの生成に失敗しました");
            return false;
        }

        maxDescriptorCount_ = numDescriptors;
        for (UINT i = 0; i < maxDescriptorCount_; ++i) {
            freeIndices_.push_back(i);
        }

        return true;
    }

    void applyPendingFree() noexcept {
        if (pendingFreeIndices_.empty()) {
            return;
        }

        for (auto i : pendingFreeIndices_) {
            freeIndices_.push_back(i);
        }
        pendingFreeIndices_.clear();
    }

    [[nodiscrad]] ID3D12DescriptorHeap* get() const noexcept {
        if (heap_) {
            assert(false && "ディスクリプタヒープが未生成です");
        }
        return heap_.Get();
    }

    [[nodiscrad]] D3D12_DESCRIPTOR_HEAP_TYPE getType() const noexcept {
        if (!heap_) {
            assert(false && "ディスクリプタヒープが未生成です");
        }
        return type_;
    }

    [[nodiscrad]] std::optional<UINT> allocateDescriptor() noexcept {
        if (freeIndices_.empty()) {
            return std::nullopt;
        }
        const auto index = freeIndices_.back();
        freeIndices_.pop_back();
        return index;
    }

    void releaseDescriptor(UINT descriptorIndex) noexcept {
        pendingFreeIndices_.push_back(descriptorIndex);
    }

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_{};
    D3D12_DESCRIPTOR_HEAP_TYPE                   type_{};
    UINT                                         maxDescriptorCount_{};
    std::vector<UINT>                            freeIndices_{};
    std::vector<UINT>                            pendingFreeIndices_{};
};

descriptor_heap_container::descriptor_heap_container() = default;

descriptor_heap_container::~descriptor_heap_container() {
    map_.clear();
}

[[nodiscard]] bool descriptor_heap_container::create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisble) noexcept {
    if (map_.find(type) != map_.end()) {
        return false;
    }

    auto p = std::make_unique<descriptor_heap>();
    if (p->create(type, numDescriptors, shaderVisble))
    {
        map_.emplace(type, std::move(p));
    }

    return true;
}

void descriptor_heap_container::applyPendingFree() noexcept 
{
    for (auto& [key, p] : map_) {
        p->applyPendingFree();
    }
}

[[nodiscard]] ID3D12DescriptorHeap* descriptor_heap_container::get(D3D12_DESCRIPTOR_HEAP_TYPE type)const noexcept
{
    const auto it = map_.find(type);
    if (it == map_.end())
    {
        assert(false && "ディスクリプタヒープがありません");
        return nullptr;
    }

    return it->second->get();
}

[[nodiscard]] std::optional<UINT> descriptor_heap_container::allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept 
{
    const auto it = map_.find(type);
    if (it == map_.end()) 
    {
        assert(false && "ディスクリプタヒープがありません");
        return std::nullopt;
    }

    return it->second->allocateDescriptor();
}

void descriptor_heap_container::releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex) noexcept 
{
    const auto it = map_.find(type);
    if (it == map_.end()) 
    {
        assert(false && "ディスクリプタヒープがありません");
    }

    return it->second->releaseDescriptor(descriptorIndex);
}