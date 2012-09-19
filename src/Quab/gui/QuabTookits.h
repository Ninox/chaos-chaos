#ifndef QUAB_TOOKITS_H
#define QUAB_TOOKITS_H

#include "../QuabDef.h"

namespace Quab
{
    namespace Widgets
    {
        class QUAB_API QuabWidget
        {
        public:
            Real width;
            Real height;
            Real widgetID;
        protected:

        };

        class QUAB_API QwEnergyBar:public QuabWidget
        {

        };

        class QUAB_API QwLabel:public QuabWidget
        {

        };

        class QUAB_API QwTextView:public QuabWidget
        {

        };

        class QUAB_API QwListView:public QuabWidget
        {

        };

        class QUAB_API QwPanel:public QuabWidget
        {

        };

        class QUAB_API QwTableView:public QuabWidget
        {

        };

        class QUAB_API QwLoadingBar:public QuabWidget
        {

        };
    }
}

#endif
