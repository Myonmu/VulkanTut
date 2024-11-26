//
// Created by miska on 2024/11/03.
//

#include "Mesh.h"

#include <VulkanAppContext.h>
#include <VulkanFrame.h>

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

    recorder.clear();

    // TODO: same pipeline could be batched
    recorder.enqueueCommand<BindPipeline>(materialInstance.getPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);

    recorder.enqueueCommand<BindVertexBuffer>(vertexBuffer);
    recorder.enqueueCommand<BindIndexBuffer>(indexBuffer);

    // bind per-frame set
    auto& perFrameSet = *ctx.renderer->getCurrentFrame().get_perFrameDescriptorSet().perFrameSet;
    recorder.enqueueCommand<BindDescriptorSet>(materialInstance.getPipelineLayout(),perFrameSet, PER_FRAME_SET_ID);

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
    :RenderingBase(ctx, materialInstance), meshBuffer(meshBuffer){
}

void MeshRenderer::enqueueDrawCall(RenderingContext &ctx, RenderPassRecorder &renderPassRecorder) {
    recorder.clear();
    enqueueSharedCommands(ctx, renderPassRecorder);
    recorder.enqueueCommand<BindMeshBuffer>(meshBuffer);
    recorder.enqueueCommand<PushConstants>(materialInstance.getPipelineLayout(),
                                           VK_SHADER_STAGE_VERTEX_BIT,
                                           sizeof(PerObjectVertexPushConstants), 0,
                                           &vertexPushConstants);
    recorder.enqueueCommand<DrawIndexed>(meshBuffer.getIndicesCount());
    renderPassRecorder.enqueueCommand<EnqueueRecorder>(recorder);
}
