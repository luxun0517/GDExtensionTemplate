//
// Created by Harris.Lu on 2024/2/3.
//

#ifndef CESIUM_GODOT_CESIUMCREDITSYSTEM_H
#define CESIUM_GODOT_CESIUMCREDITSYSTEM_H

#include <CesiumUtility/CreditSystem.h>

namespace CesiumForGodot {

    class CesiumCreditSystem {
    public:
        static CesiumCreditSystem *GetDefaultCreditSystem();

        const std::shared_ptr<CesiumUtility::CreditSystem> &GetExternalCreditSystem() const
        {
            return _pCreditSystem;
        }

    private:
        std::shared_ptr<CesiumUtility::CreditSystem> _pCreditSystem;
    };

} // CesiumForGodot

#endif //CESIUM_GODOT_CESIUMCREDITSYSTEM_H
