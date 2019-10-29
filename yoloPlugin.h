/*
created by wzq 2019 10.24
 */
/*
 * Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TRT_Yolo_PLUGIN_H
#define TRT_Yolo_PLUGIN_H
#include "cudnn.h"
#include <cublas_v2.h>
#include <string>
#include <vector>
#include <NvInferRuntimeCommon.h>

namespace nvinfer1
{
namespace plugin
{

class Yolo : public IPluginV2Ext
{
public:
    Yolo(int numclass, int stride, int gridesize, int numanchors);

    Yolo(const void* buffer, size_t length);

    ~Yolo() override = default;

    int getNbOutputs() const override;

    Dims getOutputDimensions(int index, const Dims* inputs, int nbInputDims) override;

    int initialize() override;

    void terminate() override;

    size_t getWorkspaceSize(int maxBatchSize) const override;

    int enqueue(
        int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream) override;

    size_t getSerializationSize() const override;

    void serialize(void* buffer) const override;

    bool supportsFormat(DataType type, PluginFormat format) const override;

    const char* getPluginType() const override;

    const char* getPluginVersion() const override;

    void destroy() override;

    IPluginV2Ext* clone() const override;

    void setPluginNamespace(const char* pluginNamespace) override;

    const char* getPluginNamespace() const override;

    DataType getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const override;

    bool isOutputBroadcastAcrossBatch(int outputIndex, const bool* inputIsBroadcasted, int nbInputs) const override;

    bool canBroadcastInputAcrossBatch(int inputIndex) const override;

    void attachToContext(
        cudnnContext* cudnnContext, cublasContext* cublasContext, IGpuAllocator* gpuAllocator) override;

    void configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
        const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
        const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize) override;

    void detachFromContext() override;

private:
    const char* mPluginNamespace;

    int numclass_;
    int numanchors_;
    int stride_;
    int gridesize_;

};

class YoloPluginCreator : public nvinfer1::IPluginCreator
{
public:
    YoloPluginCreator();

    ~YoloPluginCreator() override = default;

    const char* getPluginName() const override;

    const char* getPluginVersion() const override;

    const PluginFieldCollection* getFieldNames() override;

    IPluginV2Ext* createPlugin(const char* name, const PluginFieldCollection* fc) override;

    IPluginV2Ext* deserializePlugin(const char* name, const void* serialData, size_t serialLength) override;


    virtual void setPluginNamespace(const char* pluginNamespace) override{
        mNamespace = pluginNamespace;
    }

    virtual const char* getPluginNamespace() const override{
        return mNamespace.c_str();
    }

private:
    static PluginFieldCollection mFC;
    int numclass_;
    int numanchors_;
    int stride_;
    int gridesize_;
    static std::vector<PluginField> mPluginAttributes;
    std::string mNamespace;
};
} // namespace plugin
} // namespace nvinfer1

#endif // TRT_Yolo_PLUGIN_H
