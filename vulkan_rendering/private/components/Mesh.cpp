//
// Created by miska on 2024/11/03.
//

#include "Mesh.h"

#include "CBC_Drawing.h"
#include "CBC_Misc.h"

MeshRenderer::MeshRenderer(const Entity* e, const MeshBuffer &meshBuffer, const MaterialInstance &materialInstance)
    : Component(e),
      meshBuffer(meshBuffer),
      materialInstance(materialInstance) {
    // TODO: same pipeline could be batched
    recorder.enqueueCommand<BindPipeline>(materialInstance.getPipeline(), VK_PIPELINE_BIND_POINT_GRAPHICS);

    recorder.enqueueCommand<BindMeshBuffer>(meshBuffer);
    for (auto &[setId, set]: materialInstance.descriptorSets) {
        recorder.enqueueCommand<CBC_Drawing>(materialInstance.getPipelineLayout(), *set, setId);
    }
    recorder.enqueueCommand<DrawIndexed>(meshBuffer.getIndicesCount());
}

void MeshRenderer::enqueueDrawCall(RenderPassRecorder &renderPassRecorder) {
    renderPassRecorder.enqueueCommand<EnqueueRecorder>(recorder);
}
