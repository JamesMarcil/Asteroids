#ifndef NO_EFFECT_H
#define NO_EFFECT_H

#include "ImageEffect.h"

class NoEffect : public ImageEffect
{
public:
	NoEffect();
	virtual ~NoEffect(void);
	virtual void RenderEffect(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* pDeviceContext) override;
private:

};
#endif
