#ifndef QUAB_TOOKITS_H
#define QUAB_TOOKITS_H

#include "../QuabDef.h"

namespace Quab
{
    namespace Widgets
    {
        class QuabWidget
        {
        public:
            Real width;
            Real height;
            Real widgetID;
        protected:

        };

        class QwEnergyBar:public QuabWidget
        {

        };

        class QwLabel:public QuabWidget
        {

        };

        class QwTextView:public QuabWidget
        {

        };

        class QwListView:public QuabWidget
        {

        };

        class QwPanel:public QuabWidget
        {

        };

        class QwTableView:public QuabWidget
        {

        };

        class QwLoadingBar:public QuabWidget
        {

        };
    }
}

#endif
