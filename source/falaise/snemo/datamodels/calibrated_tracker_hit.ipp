// -*- mode: c++ ; -*-
/// \file falaise/snemo/datamodels/calibrated_tracker_hit.ipp

#ifndef FALAISE_SNEMO_DATAMODELS_CALIBRATED_TRACKER_HIT_IPP
#define FALAISE_SNEMO_DATAMODELS_CALIBRATED_TRACKER_HIT_IPP 1

// Ourselves:
#include <falaise/snemo/datamodels/calibrated_tracker_hit.h>

// Third party:
// - Boost:
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/nvp.hpp>
// - Bayeux/datatools:
#include <datatools/utils.h>
// - Bayeux/geomtools:
#include <geomtools/base_hit.ipp>

namespace snemo {

namespace datamodel {

template <class Archive>
void calibrated_tracker_hit::serialize(Archive& ar_, const unsigned int version_) {
    ar_& BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_hit);
    if (version_ == 0 && Archive::is_loading::value) {
        _traits_ = 0x0;
        ar_& boost::serialization::make_nvp("r", _r_);
        ar_& boost::serialization::make_nvp("sigma_r", _sigma_r_);
        ar_& boost::serialization::make_nvp("z", _z_);
        ar_& boost::serialization::make_nvp("sigma_z", _sigma_z_);
        datatools::invalidate(_x_);
        datatools::invalidate(_y_);
        ar_& boost::serialization::make_nvp("x", _x_);
        ar_& boost::serialization::make_nvp("y", _y_);
        if (datatools::is_valid(_x_) && datatools::is_valid(_y_)) {
            _set_trait_bit(true, xy);
        }
        datatools::invalidate(_delayed_time_);
        datatools::invalidate(_delayed_time_error_);
        return;
    }

    // From version 1 :
    ar_& boost::serialization::make_nvp("traits", _traits_);
    ar_& boost::serialization::make_nvp("r", _r_);
    ar_& boost::serialization::make_nvp("sigma_r", _sigma_r_);
    ar_& boost::serialization::make_nvp("z", _z_);
    ar_& boost::serialization::make_nvp("sigma_z", _sigma_z_);
    if (has_xy()) {
        ar_& boost::serialization::make_nvp("x", _x_);
        ar_& boost::serialization::make_nvp("y", _y_);
    }
    if (is_delayed()) {
        ar_& boost::serialization::make_nvp("delayed_time", _delayed_time_);
        ar_& boost::serialization::make_nvp("delayed_time_error", _delayed_time_error_);
    }
    
    // Version 2:
    ar_& boost::serialization::make_nvp("anode_time", _anode_time_);
    ar_& boost::serialization::make_nvp("sigma_anode_time", _sigma_anode_time_);
    ar_& boost::serialization::make_nvp("top_cathode_time", _top_cathode_time_);
    ar_& boost::serialization::make_nvp("sigma_top_cathode_time", _sigma_top_cathode_time_);
    ar_& boost::serialization::make_nvp("bottom_cathode_time", _bottom_cathode_time_);
    ar_& boost::serialization::make_nvp("sigma_bottom_cathode_time", _sigma_bottom_cathode_time_);

    // Version 3: Added plasma propagation time
    ar_& boost::serialization::make_nvp("plasma_propagation_time", _plasma_propagation_time_);
    // TODO: sigma

    // Version 4: Added longitudinal position
    ar_& boost::serialization::make)nvp("longitudinal_position", _longitudinal_position_);
    ar_& boost::serialization::make)nvp("sigma_longitudinal_position", _sigma_longitudinal_position_);

    return;
}

}  // end of namespace datamodel

}  // end of namespace snemo

#endif  // FALAISE_SNEMO_DATAMODELS_CALIBRATED_TRACKER_HIT_IPP
