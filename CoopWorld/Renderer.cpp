#include "Renderer.h"



Renderer::Renderer( ID3D11DeviceContext* context, EffectManager* fxMan ) :
    mD3DDeviceContext( context ),
    mFXMan( fxMan )
{
}

void Renderer::Draw( RenderItem* itemToDraw, const Scene& scene, const Camera& camera )
{
    XMMATRIX world = itemToDraw->BuildWorldMatrix();
    XMMATRIX viewProj = camera.ViewProj();
    XMMATRIX wInvTrans = D3DUtilities::InvTranspose( world );
    mFXMan->BasicFX->SetWorld( XMMatrixTranspose( world ) );
    mFXMan->BasicFX->SetWorldInvTranspose( XMMatrixTranspose( wInvTrans ) );
    mFXMan->BasicFX->SetWorldViewProj( XMMatrixTranspose( XMMatrixMultiply( world, viewProj ) ) );
    mFXMan->BasicFX->SetTexTransform( XMMatrixTranspose( itemToDraw->GetTexTransform() ) );

    if ( itemToDraw->GetMesh()->HasTexture() != 0 )
    {
        mFXMan->BasicFX->SetShaderResourceView( itemToDraw->GetMesh()->GetTexture()->GetShaderResourceView() );
    }

    mFXMan->BasicFX->SetFogColor( scene.FogColor );
    mFXMan->BasicFX->SetFogRange( scene.FogRange );
    mFXMan->BasicFX->SetFogStart( scene.FogStart );
    mFXMan->BasicFX->SetLights( ( DirectionalLight* )scene.Lights, scene.LightCount.x );
    mFXMan->BasicFX->SetEyePos( camera.GetPosition() );
    mFXMan->BasicFX->Apply( mD3DDeviceContext );
    itemToDraw->GetMesh()->Apply( mD3DDeviceContext );
    mRenderItemList.push_back( itemToDraw );
}

void Renderer::Post()
{
    mRenderItemList.clear();
}
