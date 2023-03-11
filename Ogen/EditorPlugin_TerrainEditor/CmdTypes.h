#pragma once

typedef vector<float>::type FloatValueList;
typedef map<long, float>::type FloatMap;
typedef map<long, FloatMap>::type TerrainHFMap;
typedef map<Terrain*, TerrainHFMap>::type TerrainHFData;

typedef map<size_t, float>::type BlendValue;
typedef map<size_t, BlendValue>::type BlendValueMap;
typedef map<TerrainLayerBlendMap*, BlendValueMap>::type LayerBlendValueMap;
