#ifndef WARP_EFFECT_H
#define WARP_EFFECT_H

#include "ImageEffect.h"

class Warp : public ImageEffect
{
public:
	Warp();
	virtual ~Warp(void);
	virtual void RenderEffect(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* pDeviceContext) override;
private:

};
#endif
