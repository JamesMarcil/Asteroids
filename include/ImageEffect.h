#ifndef IMAGE_EFFECT_H
#define IMAGE_EFFECT_H

#include <d3d11.h>
#include "SimpleShader.h"

class ImageEffect
{
public:
	virtual ~ImageEffect(void) {}

	virtual void RenderEffect(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* pDeviceContext) = 0;
	virtual bool Enabled() { return enabled; }
	virtual void Enabled(bool value) { enabled = value; }
protected:
	ISimpleShader* ppVS;
	ISimpleShader* ppPS;
	bool enabled;
};

#endif