//
// Created by Tereza on 10/29/2024.
//

#include "MaterialInstance.h"

#include "Material.h"

MaterialInstance::MaterialInstance(Material &material) :
srcMaterial(material), ctx(material.ctx)
{

}
