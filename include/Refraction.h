#ifndef REFRACTION_EFFECT_H
#define REFRACTION_EFFECT_H

#include "IEffect.h"

class Refraction : public IEffect
{
public:
	Refraction();
	virtual ~Refraction(void);
	virtual void RenderEffect(ID3D11ShaderResourceView* srv, ID3D11DeviceContext* pDeviceContext, float dt, float tt) override;
	virtual void Enabled(bool value) override;
private:

	ID3D11ShaderResourceView* noise;
};
#endif
