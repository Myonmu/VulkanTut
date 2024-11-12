//
// Created by miska on 2024/11/03.
//

#include "Mesh.h"
#include "CBC_Drawing.h"
#include "CBC_Misc.h"
#include "RenderingContext.h"

MeshRendererSplitBuffer::MeshRendererSplitBuffer(
    DeviceContext &ctx, const VertexBuffer &vertexBuffer,
    const IndexBuffer &indexBuffer,
    const MaterialInstance &materialInstance)
    : vertexBuffer(vertexBuffer),
      indexBuffer(indexBuffer),
      materialInstance(materialInstance) {
}

void MeshRendererSplitBuffer::enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) {
    auto frameInfo = ctx.renderer->getCurrentFrameInfo();

    recorder.clear();

    // TODO: same pipeline could be batched
    recorder.enqueueCommand<BindPipeline>(materialInstance.getPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);

    recorder.enqueueCommand<BindVertexBuffer>(vertexBuffer);
    recorder.enqueueCommand<BindIndexBuffer>(indexBuffer);

    recorder.enqueueCommand<BindDescriptorSet>(materialInstance.getPipelineLayout(),
                                               *ctx.perFrameSets[frameInfo.currentFrameIndex], 0);

    for (auto &[setId, set]: materialInstance.descriptorSets) {
        if (setId <= 0)continue; // 0 is reserved for global set
        recorder.enqueueCommand<BindDescriptorSet>(materialInstance.getPipelineLayout(), *set, setId);
    }
    recorder.enqueueCommand<PushConstants>(materialInstance.getPipelineLayout(),
                                           VK_SHADER_STAGE_VERTEX_BIT,
                                           sizeof(PerObjectVertexPushConstants), 0,
                                           &vertexPushConstants);
    recorder.enqueueCommand<DrawIndexed>(indexBuffer.getIndicesCount());
    renderPassRecorder.enqueueCommand<EnqueueRecorder>(recorder);
}

MeshRenderer::MeshRenderer(DeviceContext &ctx, const MeshBuffer &meshBuffer, const MaterialInstance &materialInstance)
    : meshBuffer(meshBuffer),
      materialInstance(materialInstance) {
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
    recorder.enqueueCommand<PushConstants>(materialInstance.getPipelineLayout(),
                                           VK_SHADER_STAGE_VERTEX_BIT,
                                           sizeof(PerObjectVertexPushConstants), 0,
                                           &vertexPushConstants);
    recorder.enqueueCommand<DrawIndexed>(meshBuffer.getIndicesCount());
    renderPassRecorder.enqueueCommand<EnqueueRecorder>(recorder);
}
