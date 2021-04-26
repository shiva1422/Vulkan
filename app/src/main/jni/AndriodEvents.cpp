//
// Created by pspr1 on 4/20/2021.
//

#include "AndriodEvents.h"
#include "Commons.h"
#include "Vulkan.h"

void AndriodEvents::onCmd(android_app *app, int32 cmd)
{
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
        {

        }break;

        case APP_CMD_INIT_WINDOW:
        {
            Vulkan::initialize(app);
          //  hideSystemUI(app);
            //  CallbacksLog("the window h is %d and the window w is %d",ANativeWindow_getHeight(app->window),ANativeWindow_getWidth(app->window));
        }
            break;
        case APP_CMD_TERM_WINDOW:
            {
            //AppContext  *appContext=((PhotoApp *)app->userData)->appContext;
        //    appContext->windowInitStatus=false;

        }
            break;
        case APP_CMD_LOST_FOCUS:

            break;
        case APP_CMD_GAINED_FOCUS:

            break;
        default:
        {
            Loge("ONCMD","default case:implememnt this case");
        }
    }


}