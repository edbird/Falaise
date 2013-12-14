/* calo_locator.h
 * Author(s)     :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2011-05-05
 * Last modified : 2013-04-19
 *
 * Copyright (C) 2011-2013 Francois Mauger <mauger@lpccaen.in2p3.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Description:
 *
 *   Calorimeter scintillator block locator for one module of the SuperNEMO detector
 *
 * History:
 *
 */

#ifndef FLSNGEOMETRY_CALO_LOCATOR_H_
#define FLSNGEOMETRY_CALO_LOCATOR_H_ 1

#include <string>

#include <boost/cstdint.hpp>

#include <geomtools/i_locator.h>

/** forward declaration */
namespace geomtools {
  class mapping;
  class id_mgr;
  class manager;
  class placement;
  class i_shape_3d;
  class box;
}

namespace snemo {

  namespace geometry {

    class calo_locator : public geomtools::base_locator
    {
    public:

      static const size_t NSIDES      = 2;
      static const size_t NSUBMODULES = NSIDES;
      static const int    BACK_SIDE   = 0; // Back submodule
      static const int    FRONT_SIDE  = 1; // Front submodule

      // For geometry config snemo::demonstrator >=2.0
      static const int BLOCK_BACK_PART    = 0; // Back volume of scintillator (possibly extruded)
      static const int BLOCK_FRONT_PART   = 1; // Front volume of scintillator (entrance window)
      static const int DEFAULT_BLOCK_PART = BLOCK_FRONT_PART;

      /*
       *  [s][s][s][s][s]
       *  [s][D][S][D][s]
       *  [s][S][x][S][s]
       *  [s][D][S][D][s]
       *  [s][s][s][s][s]
       *
       */
      enum neighbour_mask_type
        {
          NEIGHBOUR_NONE = 0x0,
          NEIGHBOUR_SIDE = 0x1,   // S : nearest neighbours (side)
          NEIGHBOUR_DIAG = 0x2,   // D : diagonal
          //NEIGHBOUR_SECOND = 0x4, // s : second ranked neighbours (not used)
          NEIGHBOUR_FIRST = NEIGHBOUR_SIDE | NEIGHBOUR_DIAG
        };

      virtual bool is_initialized () const;

      /// Constructor
      calo_locator ();

      /// Constructor:
      calo_locator (const ::geomtools::manager & mgr_,
                    uint32_t module_number_,
                    uint32_t block_part_ = DEFAULT_BLOCK_PART);

      /// Destructor
      virtual ~calo_locator ();

      virtual void initialize (const datatools::properties & config_);

      virtual void reset ();

      void initialize ();

      bool has_submodule (uint32_t side_) const;

      bool id_is_valid (uint32_t side_, uint32_t column_, uint32_t row_) const;

      // Interfaces from geomtools::i_locator :
      virtual bool find_geom_id (const geomtools::vector_3d & world_position_,
                                 int type_,
                                 geomtools::geom_id & gid_,
                                 double tolerance_ = GEOMTOOLS_PROPER_TOLERANCE) const;

      bool find_block_geom_id (const geomtools::vector_3d & world_position_,
                               geomtools::geom_id & gid_,
                               double tolerance_ = GEOMTOOLS_PROPER_TOLERANCE) const;
    protected:

      /**
       */
      bool find_block_geom_id_ (const geomtools::vector_3d & in_module_position_,
                                geomtools::geom_id & gid_,
                                double tolerance_ = GEOMTOOLS_PROPER_TOLERANCE);

      void _set_defaults_ ();

    public:
      /**! @return the number of the module (>=0).
       */
      uint32_t get_module_number () const;

      /**! set the number of the module for this locator.
       */
      void set_module_number (uint32_t module_number_);

      /**! set the block part number for this locator.
       */
      void set_block_part (uint32_t block_part_);

      /**! return the block part number for this locator.
       */
      uint32_t get_block_part () const;

      /**! check if block is partitioned in the current setup.
       */
      bool is_block_partitioned () const;

      void dump (std::ostream & out_ = std::clog) const;

      /**! @return the width of a calorimeter block.
       */
      double get_block_width () const;

      /**! @return the height of a calorimeter block.
       */
      double get_block_height () const;

      /**! @return the thickness of a block.
       */
      double get_block_thickness () const;

      /**! @return the number of sides on the calorimeter.
       */
      size_t get_number_of_sides () const;

      /**! @return the number of columns in one side of the calorimeter.
       */
      size_t get_number_of_columns (uint32_t side_) const;

      /**! @return the number of rows in one side of the calorimeter.
       */
      size_t get_number_of_rows (uint32_t side_) const;

      /**! @return the X-position of a wall for specific side (in module coordinate system).
       */
      double get_wall_x (uint32_t side_) const;

      /**! @return the X-position of the entrance window of a wall for specific side (in module coordinate system).
       */
      double get_wall_window_x (uint32_t side_) const;

      /**! @return the Y-position of a block for specific side and column (in module coordinate system).
       */
      double get_column_y (uint32_t side_, uint32_t column_) const;

      /**! @return the Z-position of a block for specific side and row (in module coordinate system).
       */
      double get_row_z (uint32_t side_, uint32_t row_) const;

      /**! Compute the position of a block
       *   for specific side, column and row (in module coordinate system).
       */
      void compute_block_position (uint32_t side_, uint32_t column_, uint32_t row_, geomtools::vector_3d & module_position_) const;

      /**! Compute the position of the center of the entrance window of a block
       *   for specific side, column and row (in module coordinate system).
       */
      void compute_block_window_position (uint32_t side_, uint32_t column_, uint32_t row_, geomtools::vector_3d & module_position_) const;

      /**! @return the position of a block for specific side, column and row (in module coordinate system).
       */
      geomtools::vector_3d get_block_position (uint32_t side_, uint32_t column_, uint32_t row_) const;

      /**! @return the position of the center of the entrance window of a block
       *   for specific side, column and row (in module coordinate system).
       */
      geomtools::vector_3d get_block_window_position (uint32_t side_, uint32_t column_, uint32_t row_) const;

      /** Tranform a world coordinate system position to the corresponding module coordinate system position.
       */
      void transform_world_to_module (const geomtools::vector_3d & world_position_,
                                      geomtools::vector_3d & module_position_) const;

      /** Tranform a module coordinate system position to the corresponding world coordinate system position.
       */
      void transform_module_to_world (const geomtools::vector_3d & module_position_,
                                      geomtools::vector_3d & world_position_) const;


      /** Check if a world coordinate system position is in the module virtual volume (its bounding envelope).
       */
      bool is_world_position_in_module (const geomtools::vector_3d & world_position_,
                                        double tolerance_ = GEOMTOOLS_PROPER_TOLERANCE) const;


      /** Check if a module coordinate system position is in the module virtual volume (its bounding envelope).
       */
      bool is_in_module (const geomtools::vector_3d & module_position_,
                         double tolerance_ = GEOMTOOLS_PROPER_TOLERANCE) const;


      /** Check if a module coordinate system position is in a specific block virtual volume (its bounding envelope).
       */
      bool is_in_block (const geomtools::vector_3d & module_position_,
                        uint32_t side_,
                        uint32_t column_,
                        uint32_t row_,
                        double tolerance_ = GEOMTOOLS_PROPER_TOLERANCE) const;


      /** Check if a world coordinate system position is in a specific block virtual volume (its bounding envelope).
       */
      bool is_world_position_in_block (const geomtools::vector_3d & world_position_,
                                       uint32_t side_,
                                       uint32_t column_,
                                       uint32_t row_,
                                       double tolerance_ = GEOMTOOLS_PROPER_TOLERANCE) const;

      /** Given a block at specific side, column and row, returns the number of neighbouring blocks.
       */
      size_t get_number_of_neighbours (uint32_t side_,
                                       uint32_t column_,
                                       uint32_t row_,
                                       uint8_t mask_ = NEIGHBOUR_FIRST) const;

      /** Given a block with a specific geometry ID, returns the number of neighbouring blocks.
       */
      size_t get_number_of_neighbours (const geomtools::geom_id & gid_,
                                       uint8_t mask_ = NEIGHBOUR_FIRST) const;

      /** Given a block at specific side, column and row, compute the array of geometry IDs for associated neighbouring blocks.
       */
      void get_neighbours_ids (uint32_t side_,
                               uint32_t column_,
                               uint32_t row_,
                               std::vector<geomtools::geom_id> & ids_,
                               uint8_t mask_ = NEIGHBOUR_FIRST) const;

      /** Given a block with a specific geometry IDs, compute the array of geometry IDs for associated neighbouring blocks.
       */
      void get_neighbours_ids (const geomtools::geom_id & gid_,
                               std::vector<geomtools::geom_id> & ids_,
                               uint8_t mask_ = NEIGHBOUR_FIRST) const;

      /** Given a block with a specific geometry IDs, compute its position in the module coordinate system.
       */
      void get_block_position (const geomtools::geom_id & gid_,
                               geomtools::vector_3d & position_) const;


      /** Given a block with a specific side, column and row, compute its position in the module coordinate system.
       */
      void get_block_position (uint32_t side_,
                               uint32_t column_,
                               uint32_t row_,
                               geomtools::vector_3d & position_) const;

      int get_module_address_index () const;

      int get_side_address_index () const;

      int get_column_address_index () const;

      int get_row_address_index () const;

      int get_part_address_index () const;

      uint32_t extract_module (const geomtools::geom_id & gid_) const;

      uint32_t extract_side (const geomtools::geom_id & gid_) const;

      uint32_t extract_column (const geomtools::geom_id & gid_) const;

      uint32_t extract_row (const geomtools::geom_id & gid_) const;

      uint32_t extract_part (const geomtools::geom_id & gid_) const;

      bool is_calo_block (const geomtools::geom_id & gid_) const;

      bool is_calo_block_in_current_module (const geomtools::geom_id & gid_) const;

    protected:

      void _construct ();

    private:

      bool _initialized_;

      // Configuration parameters :
      uint32_t        _module_number_;
      uint32_t        _block_part_;

      // Running values :
      uint32_t    _module_type_;
      uint32_t    _calorimeter_submodule_type_;
      uint32_t    _block_type_;
      uint32_t    _wrapper_type_;
      bool        _block_partitioned_;

      const geomtools::mapping    * _mapping_;
      const geomtools::id_mgr     * _id_manager_;

      const geomtools::geom_info  * _module_ginfo_;
      const geomtools::placement  * _module_world_placement_;
      const geomtools::box        * _module_box_;
      const geomtools::i_shape_3d * _block_shape_;
      bool                          _composite_block_shape_;
      const geomtools::box        * _block_box_;
      double              _block_x_[NSIDES];
      double              _block_window_x_[NSIDES];
      std::vector<double> _back_block_z_;
      std::vector<double> _front_block_z_;
      std::vector<double> _back_block_y_;
      std::vector<double> _front_block_y_;

      double _block_width_;
      double _block_height_;
      double _block_thickness_;

      // Block GID addressing :
      int _module_address_index_;
      int _side_address_index_;
      int _column_address_index_;
      int _row_address_index_;
      int _part_address_index_;

      // Submodules are present :
      bool _submodules_[NSIDES];

    };

  }  // end of namespace geometry

}  // end of namespace snemo

#endif // FLSNGEOMETRY_CALO_LOCATOR_H_

// end of calo_locator.h
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
