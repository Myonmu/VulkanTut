//
// Created by miska on 2024/11/03.
//

#include "Mesh.h"
#include "CBC_Drawing.h"
#include "CBC_Misc.h"
#include "RenderingContext.h"

void PerObjectBuffer::updatePerObjectBuffer(
    const FrameInfo &frameInfo,
    const Position &pos,
    const Rotation &rot,
    const Scale &scale) const {
    auto const &buffer = (*bufferGroup)[frameInfo.currentFrameIndex];
    const UniformBufferObject ubo{
        .model = Transform::getModelMatrix(pos, rot, scale)
    };
    buffer.copyToBufferMemory(&ubo, 0);
}

MeshRenderer::MeshRenderer(DeviceContext& ctx,const MeshBuffer &meshBuffer, const MaterialInstance &materialInstance)
    : meshBuffer(meshBuffer),
      materialInstance(materialInstance) {
    perObjectBuffer = std::make_unique<PerObjectBuffer>(ctx);
}

void MeshRenderer::enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) {
    auto frameInfo = ctx.renderer->getCurrentFrameInfo();

    recorder.clear();

    // TODO: same pipeline could be batched
    recorder.enqueueCommand<BindPipeline>(materialInstance.getPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);

    recorder.enqueueCommand<BindMeshBuffer>(meshBuffer);

    recorder.enqueueCommand<BindDescriptorSet>(materialInstance.getPipelineLayout(),
                                               *ctx.perFrameSets[frameInfo.currentFrameIndex], 0);

    for (auto &[setId, set]: materialInstance.descriptorSets) {
        if (setId <= 0)continue; // 0 is reserved for global set
        recorder.enqueueCommand<BindDescriptorSet>(materialInstance.getPipelineLayout(), *set, setId);
    }
    recorder.enqueueCommand<DrawIndexed>(meshBuffer.getIndicesCount());
    renderPassRecorder.enqueueCommand<EnqueueRecorder>(recorder);
}

void MeshRenderer::updatePerObjectDescriptorSet(RenderingContext &ctx) {
    auto frameInfo = ctx.renderer->getCurrentFrameInfo();
    auto const& buffer = (*perObjectBuffer->bufferGroup)[frameInfo.currentFrameIndex];
    DescriptorWriter writer{};
    writer.writeBuffer(0, buffer, sizeof(UniformBufferObject), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    writer.updateSet(ctx.getLogicalDevice(), materialInstance.getDescriptorSet(1));
}
