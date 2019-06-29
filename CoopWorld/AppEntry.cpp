#include "DemoApp.h"

#include <exception>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd )
{
    #if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    #endif
    DemoApp app( hInstance );

    try
    {
        if ( !app.Initialize() )
        {
            return -1;
        }
		
        //app.Set4xMsaaState( true );
        return app.Run();
    }
    catch ( DXException& err )
    {
        MessageBox( nullptr, err.ToString().c_str(), L"Oops", MB_OK );
		return -1;
    }
	catch (std::exception& syserr)
	{
		MessageBoxA(nullptr, syserr.what(), "Oops", MB_OK);
		return -1;
	}
}