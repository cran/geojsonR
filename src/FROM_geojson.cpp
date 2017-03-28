
/**
 * Copyright (C) 2017 Lampros Mouselimis
 *
 * @file FROM_geojson.cpp
 *
 * @author Lampros Mouselimis
 *
 * @date February - March 2017
 *
 * @Notes: reads GeoJson from file / url / character-string
 *
 * @last_modified: March 2017
 *
 **/


# include <RcppArmadillo.h>
// [[Rcpp::depends("RcppArmadillo")]]
// [[Rcpp::plugins(cpp11)]]


#define NDEBUG                              // disable assertions completely, due to '__assert_fail' errors when checking package [ add flag also in Makevars file ]

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <R.h>
#include <Rinternals.h>

#include "json11.h"



// class to parse geojson geometries [ except for multi-polygon ]
//


class From_GeoJson_geometries {

private:

  arma::rowvec flatten_coords_pr;


public:

  From_GeoJson_geometries() { }


  // switch (if-else) function for the geometry-objects
  //

  Rcpp::List geom_OBJ(std::string geom_OBJECT, json11::Json input_obj, int polygon_size = 1, bool average_coordinates = false) {       // 'average_coordinates' of geojson object is needed (indirectly) in leaflet's "setView()"

    Rcpp::List switch_OBJ;

    if (geom_OBJECT == "Point") {

      json11::Json::array tmp_point = input_obj["coordinates"].array_items();

      arma::mat res_point(1, 2);

      res_point(0,0) = tmp_point[0].number_value();

      res_point(0,1) = tmp_point[1].number_value();

      switch_OBJ["unlist_OBJ"] = res_point;

      if (average_coordinates) {

        flatten_coords_pr.set_size(2);

        flatten_coords_pr(0) = tmp_point[0].number_value();

        flatten_coords_pr(1) = tmp_point[1].number_value();
      }
    }

    else if (geom_OBJECT == "LineString" || geom_OBJECT == "MultiPoint" || (geom_OBJECT == "Polygon" && polygon_size == 1)) {

      json11::Json::array tmp_array;

      if (geom_OBJECT == "Polygon") {

        tmp_array = input_obj["coordinates"][0].array_items();}                 // array_item[0] in case of polygon-without-interior-rings

      if (geom_OBJECT == "LineString" || geom_OBJECT == "MultiPoint") {

        tmp_array = input_obj["coordinates"].array_items();
      }

      unsigned int size_array = tmp_array.size();

      arma::mat tmp_RES_(size_array, 2);

      for (unsigned int i = 0; i < size_array; i++) {

        json11::Json::array inner_array = tmp_array[i].array_items();

        arma::rowvec inner_vec(inner_array.size());

        for (unsigned int j = 0; j < inner_array.size(); j++) {

          inner_vec(j) = inner_array[j].number_value();
        }

        tmp_RES_.row(i) = inner_vec;
      }

      if (average_coordinates) {

        flatten_coords_pr = arma::conv_to< arma::rowvec >::from(arma::mean(tmp_RES_, 0));
      }

      switch_OBJ["unlist_OBJ"] = tmp_RES_;
    }

    else if (geom_OBJECT == "MultiLineString" || (geom_OBJECT == "Polygon" && polygon_size > 1)) {

      json11::Json::array poly_tmp = input_obj["coordinates"].array_items();

      unsigned int inner_poly_size = poly_tmp.size();

      Rcpp::List tmp_RES_poly_interior;

      arma::mat outer_avg;

      if (average_coordinates) {

        outer_avg.set_size(inner_poly_size, 2);
      }

      for (unsigned int k = 0; k < inner_poly_size; k++) {

        json11::Json::array tmp_array = poly_tmp[k].array_items();

        unsigned int size_array = tmp_array.size();

        arma::mat coord_mat(size_array, 2);

        for (unsigned int i = 0; i < size_array; i++) {

          json11::Json::array inner_array = tmp_array[i].array_items();

          arma::rowvec inner_vec(inner_array.size());

          for (unsigned int j = 0; j < inner_array.size(); j++) {

            inner_vec(j) = inner_array[j].number_value();
          }

          coord_mat.row(i) = inner_vec;
        }

        if (average_coordinates) {

          outer_avg.row(k) = arma::conv_to< arma::rowvec >::from(arma::mean(coord_mat, 0));
        }

        tmp_RES_poly_interior.push_back(coord_mat);
      }

      if (average_coordinates) {

        flatten_coords_pr = arma::conv_to< arma::rowvec >::from(arma::mean(outer_avg, 0));
      }

      switch_OBJ.push_back(tmp_RES_poly_interior);
    }

    else if (geom_OBJECT == "MultiPolygon") {

      json11::Json::array tmp_mlpol = input_obj["coordinates"].array_items();

      unsigned int outer_size = tmp_mlpol.size();

      arma::mat outer_avg;

      if (average_coordinates) {

        outer_avg.set_size(outer_size, 2);
      }

      for (unsigned int k = 0; k < outer_size; k++) {

        json11::Json array_item = tmp_mlpol[k];

        int tmp_size_array = array_item.array_items().size();

        if (tmp_size_array == 1) {

          json11::Json::array tmp_array = array_item[0].array_items();                         // array_item[0] in case of polygon-without-interior-rings  [ multi-polygon --version ]

          unsigned int size_array = tmp_array.size();

          arma::mat tmp_RES_(size_array, 2);

          for (unsigned int i = 0; i < size_array; i++) {

            json11::Json::array inner_array = tmp_array[i].array_items();

            arma::rowvec inner_vec(inner_array.size());

            for (unsigned int j = 0; j < inner_array.size(); j++) {

              inner_vec(j) = inner_array[j].number_value();
            }

            tmp_RES_.row(i) = inner_vec;
          }

          if (average_coordinates) {

            outer_avg.row(k) = arma::conv_to< arma::rowvec >::from(arma::mean(tmp_RES_, 0));
          }

          switch_OBJ.push_back(tmp_RES_);
        }

        if (tmp_size_array > 1) {

          json11::Json::array poly_tmp = array_item.array_items();                             // multi-polygon [ polygon WITH interior --version ]

          unsigned int inner_poly_size = poly_tmp.size();

          Rcpp::List tmp_RES_poly_interior;

          arma::mat outer_avg_sec;

          if (average_coordinates) {

            outer_avg_sec.set_size(inner_poly_size, 2);
          }

          for (unsigned int k1 = 0; k1 < inner_poly_size; k1++) {

            json11::Json::array tmp_array = poly_tmp[k1].array_items();

            unsigned int size_array = tmp_array.size();

            arma::mat coord_mat(size_array, 2);

            for (unsigned int i = 0; i < size_array; i++) {

              json11::Json::array inner_array = tmp_array[i].array_items();

              arma::rowvec inner_vec(inner_array.size());

              for (unsigned int j = 0; j < inner_array.size(); j++) {

                inner_vec(j) = inner_array[j].number_value();
              }

              coord_mat.row(i) = inner_vec;
            }

            if (average_coordinates) {

              outer_avg_sec.row(k1) = arma::conv_to< arma::rowvec >::from(arma::mean(coord_mat, 0));
            }

            tmp_RES_poly_interior.push_back(coord_mat);
          }

          if (average_coordinates) {

            outer_avg.row(k) = arma::conv_to< arma::rowvec >::from(arma::mean(outer_avg_sec, 0));
          }

          switch_OBJ.push_back(tmp_RES_poly_interior);
        }
      }

      if (average_coordinates) {

        flatten_coords_pr = arma::conv_to< arma::rowvec >::from(arma::mean(outer_avg, 0));
      }
    }

    else {

      Rcpp::stop("invalid GeoJson geometry object --> geom_OBJ() function");
    }

    return switch_OBJ;
  }


  // return average_coordinates
  //

  arma::rowvec return_COORDS() {

    return flatten_coords_pr;
  }


  // geometry-collection
  //

  Rcpp::List geom_collection_OBJ(json11::Json parse_geom, bool average_coordinates = false) {              // 'average_coordinates' of geojson object is needed (indirectly) in leaflet's "setView()"

    Rcpp::List RES_col;

    json11::Json::array col_array = parse_geom["geometries"].array_items();

    RES_col["type"] = parse_geom["type"].string_value();

    unsigned int SIZE = col_array.size();

    Rcpp::List geoms_tmp;

    arma::mat outer_avg;

    if (average_coordinates) {

      outer_avg.set_size(SIZE, 2);
    }

    for (unsigned int f = 0; f < SIZE; f++) {

      json11::Json iter = col_array[f];

      std::string res_type = iter["type"].string_value();

      Rcpp::List RES_inner;

      RES_inner["type"] = res_type;

      int polygon_size = iter["coordinates"].array_items().size();

      if (res_type == "Point" || res_type == "LineString" || res_type == "MultiPoint" || (res_type == "Polygon" && polygon_size == 1)) {

        RES_inner["coordinates"] = geom_OBJ(res_type, iter, polygon_size, average_coordinates)["unlist_OBJ"];         // unlist object
      }

      else {

        RES_inner["coordinates"] = geom_OBJ(res_type, iter, polygon_size, average_coordinates);
      }

      if (average_coordinates) {

        outer_avg.row(f) = flatten_coords_pr;

        flatten_coords_pr.clear();                  // each time it's called : it first assigns to arma::mat then clears the private-variable [ due to "inside-class-loop" ]
      }

      geoms_tmp.push_back(RES_inner);
    }

    if (average_coordinates) {

      flatten_coords_pr = arma::conv_to< arma::rowvec >::from(arma::mean(outer_avg, 0));
    }

    RES_col["geometries"] = geoms_tmp;

    return RES_col;
  }



  // secondary function for the "Feature" GeoJson object [ to process the "properties" member recursively ]
  //

  SEXP recursive_switch(json11::Json json) {             // json["properties"][ITEM]  AND use by default ITEM = "NULL" only in case of an array

    if (json.is_string()) {

      return Rcpp::wrap(json.string_value());}

    else if (json.is_bool()) {

      return Rcpp::wrap(json.bool_value());}

    else if (json.is_null()) {

      return Rcpp::wrap(R_NilValue);}                                                  // assign R's NULL value

    else if (json.is_number()) {

      return Rcpp::wrap(json.number_value());}

    else if (json.is_array()) {

      Rcpp::List recurs_out_array;

      for (auto& second_item : json.array_items()) {

        recurs_out_array.push_back(Rcpp::wrap(recursive_switch(second_item)));          // recursion of 'arrays' for geojson files is limited only to the "properties" member (unknown depth of array AND unknown type of item of each array). This because recursion is slower than a for loop
      }

      return recurs_out_array;
    }

    else if (json.is_object()) {

      Rcpp::List recurs_out;

      for (auto& second_item : json.object_items()) {

        std::string SEC_ITEM = second_item.first;

        recurs_out[SEC_ITEM] = Rcpp::wrap(recursive_switch(json[SEC_ITEM]));             // recursion for properties [ unknown depth ]
      }

      return recurs_out;
    }

    else {

      Rcpp::stop("invalid Json object --> recursive_switch() function");
    }
  }


  // geojson object : "Feature"
  //

  Rcpp::List feature_OBJ(json11::Json input_obj, bool flatten_coords = false, bool average_coordinates = false) {     // 'flatten_coords' and 'average_coordinates' of geojson object is needed (indirectly) in leaflet's "setView()"

    Rcpp::List RES_feat;

    for (auto& iter : input_obj.object_items()) {

      if (iter.first == "type") {

        RES_feat["type"] = iter.second.string_value();}

      else if (iter.first == "id" || iter.first == "_id") {                 // exception : some geojson feature-files have the 'id' member with an underscore [ in any case create an 'id' in the resulted list ]

        if (iter.second.is_number()) {

          RES_feat["id"] = iter.second.number_value();}

        else if (iter.second.is_string()) {

          RES_feat["id"] = iter.second.string_value();}

        else {

          Rcpp::stop("invalid type for the 'id' member --> feature_geojson_sequential() function");
        }
      }

      else if (iter.first == "bbox") {

        std::vector<json11::Json> tmp_arr = iter.second.array_items();

        std::vector<double> bbox_vec;

        for (auto& it : tmp_arr) {

          bbox_vec.push_back(it.number_value());
        }

        RES_feat["bbox"] = bbox_vec;
      }

      else if (iter.first == "geometry") {

        json11::Json tmp_geom = input_obj["geometry"];

        std::string res_type = tmp_geom["type"].string_value();

        int polygon_size = tmp_geom["coordinates"].array_items().size();

        Rcpp::List RES_OUT_feat;

        RES_OUT_feat["type"] = res_type;

        if (res_type == "Point" || res_type == "LineString" || res_type == "MultiPoint" || (res_type == "Polygon" && polygon_size == 1)) {

          RES_OUT_feat["coordinates"] = geom_OBJ(res_type, tmp_geom, polygon_size, average_coordinates)["unlist_OBJ"];}                           // unlist object

        else {

          RES_OUT_feat["coordinates"] = geom_OBJ(res_type, tmp_geom, polygon_size, average_coordinates);
        }

        RES_feat["geometry"] = RES_OUT_feat;
      }

      else if (iter.first == "properties" && !flatten_coords) {

        Rcpp::List tmp_prop;

        for (auto& item_prop : input_obj["properties"].object_items()) {

          std::string first_item = item_prop.first;

          tmp_prop[first_item] = recursive_switch(input_obj["properties"][first_item]);
        }

        RES_feat["properties"] = tmp_prop;
      }
    }

    return RES_feat;
  }


  // geojson object : "FeatureCollection"
  //

  Rcpp::List feature_collection_geojson(json11::Json input_obj, bool flatten_coords = false, bool average_coordinates = false) {         // 'flatten_coords' and 'average_coordinates' of geojson object is needed (indirectly) in leaflet's "setView()"

    Rcpp::List RES_feat_col;

    for (auto& iter : input_obj.object_items()) {

      if (iter.first == "type") {

        RES_feat_col["type"] = iter.second.string_value();}

      else if (iter.first == "bbox") {

        std::vector<double> bbox_vec;

        for (auto& num : iter.second.array_items()) {

          bbox_vec.push_back(num.number_value());
        }

        RES_feat_col["bbox"] = bbox_vec;
      }

      else if (iter.first == "features") {

        Rcpp::List lst_feats;

        json11::Json::array tmp_arr = input_obj["features"].array_items();

        unsigned int ITER_AVG = 0;

        arma::mat outer_avg;

        if (average_coordinates) {

          outer_avg.set_size(tmp_arr.size(), 2);
        }

        for (auto& itf : tmp_arr) {

          lst_feats.push_back(feature_OBJ(itf, flatten_coords, average_coordinates));

          if (average_coordinates) {

            outer_avg.row(ITER_AVG) = flatten_coords_pr;

            flatten_coords_pr.clear();                       // each time it's called : it first assigns to arma::mat then clears the private-variable [ due to "inside-class-loop" ]

            ITER_AVG++;
          }
        }

        if (average_coordinates) {

          flatten_coords_pr = arma::conv_to< arma::rowvec >::from(arma::mean(outer_avg, 0));
        }

        RES_feat_col["features"] = lst_feats;
      }
    }

    return RES_feat_col;
  }



  // check if input-file exists
  //

  bool file_exists(std::string fileName) {

    std::ifstream infile(fileName);

    return infile.good();
  }


  // parse the geo-json objects
  //

  json11::Json parse_geojson_objects(std::string input_data) {

    // if (!file_exists(input_data)) {                                                           # redundant, see the Rcpp-exported function
    //
    //   Rcpp::stop("the input file does not exist --> parse_geojson_objects() function");
    // }

    std::string data_in;

    std::fstream myfile(input_data, std::fstream::in);

    char chs;

    while (myfile >> std::noskipws >> chs) {

      data_in += chs;
    }

    std::string Error_Message = "the data is not a GeoJson object";

    json11::Json json_input = json11::Json::parse(data_in, Error_Message);

    return json_input;
  }


  // input a geojson character string (rather than a path to a file)
  //

  json11::Json parse_geojson_string(std::string character_string) {

    std::string Error_Message = "the data is not a GeoJson object";

    json11::Json json_input = json11::Json::parse(character_string, Error_Message);

    return json_input;
  }


  ~From_GeoJson_geometries() { }
};



// Rcpp-exported functions
//

//-----------------------
// FROM-GeoJson function
//-----------------------


// geometry-objects [ 'flatten_coords' AND 'average_coordinates' are used in leaflet's "setView()"  ['longitude' and 'latitude'] ]
//

// [[Rcpp::export]]
Rcpp::List export_From_geojson(std::string input_file, bool flatten_coords = false, bool average_coordinates = false) {

  Rcpp::List RES_OUT;

  From_GeoJson_geometries prs;

  json11::Json tmp_prs;

  if (prs.file_exists(input_file)) {                        // check if file exists

    tmp_prs = prs.parse_geojson_objects(input_file);}       // input is a path to a file

  else {

    tmp_prs = prs.parse_geojson_string(input_file);         // input is a geojson character string
  }

  std::string res_type = tmp_prs["type"].string_value();

  int polygon_size = tmp_prs["coordinates"].array_items().size();

  if (res_type == "GeometryCollection") {

    RES_OUT = prs.geom_collection_OBJ(tmp_prs, average_coordinates);}

  else if (res_type == "Feature") {

    RES_OUT = prs.feature_OBJ(tmp_prs, flatten_coords, average_coordinates);                      // setting 'flatten_coords' to TRUE avoids 'properties' member recursive calculation
  }

  else if (res_type == "FeatureCollection") {

    RES_OUT = prs.feature_collection_geojson(tmp_prs, flatten_coords, average_coordinates);
  }

  else if (res_type == "Point" || res_type == "LineString" || res_type == "MultiPoint" || (res_type == "Polygon" && polygon_size == 1)) {

    RES_OUT["type"] = res_type;

    RES_OUT["coordinates"] = prs.geom_OBJ(res_type, tmp_prs, polygon_size, average_coordinates)["unlist_OBJ"];}      // unlist object

  else if (res_type == "MultiLineString" || res_type == "MultiPolygon" || (res_type == "Polygon" && polygon_size > 1)) {

    RES_OUT["type"] = res_type;

    RES_OUT["coordinates"] = prs.geom_OBJ(res_type, tmp_prs, polygon_size, average_coordinates);}

  else {

    Rcpp::stop("Give a valid path to a '.geojson' file or a 'GeoJson' character string. Valid GeoJson objects are : 'Point', 'LineString', 'MultiPoint', 'Polygon', 'MultiLineString', 'MultiPolygon', 'GeometryCollection', 'Feature' and 'FeatureCollection'");
  }

  if (average_coordinates) {

    RES_OUT["geometry_dump"] = tmp_prs.dump();

    RES_OUT["leaflet_view_coords"] = prs.return_COORDS();
  }

  return RES_OUT;
}




// fully recursive extraction of data [ used in 'address_geocoding_nominatim()' AND 'reverse_geocoding_nominatim()' functions ]
//

// [[Rcpp::export]]
SEXP export_From_JSON(std::string input_file) {

  Rcpp::List RES_OUT;

  From_GeoJson_geometries prs;

  json11::Json tmp_prs;

  if (prs.file_exists(input_file)) {                        // check if file exists

    tmp_prs = prs.parse_geojson_objects(input_file);}       // input is a path to a file

  else {

    tmp_prs = prs.parse_geojson_string(input_file);         // input is a json character string
  }

  SEXP tmp = prs.recursive_switch(tmp_prs);

  return tmp;
}



// dump a geojson object
//

// [[Rcpp::export]]
std::string dump_geojson(std::string input_data) {

  From_GeoJson_geometries fgj;

  json11::Json tmp_gj = fgj.parse_geojson_objects(input_data);

  return tmp_gj.dump();
}


// FeatureCollection from geojson Feature files
//

// [[Rcpp::export]]
std::string Features_TO_Collection(std::vector<std::string> feat_files_lst, std::vector<double> bbox_vec) {

  From_GeoJson_geometries prs;

  json11::Json::array feat_col_array;

  for (unsigned int i = 0; i < feat_files_lst.size(); i++) {

    std::string input_file = feat_files_lst[i];

    json11::Json tmp_prs = prs.parse_geojson_objects(input_file);

    feat_col_array.push_back(tmp_prs);
  }

  json11::Json Geom_Coll_OBJ = json11::Json::object {

    { "type", "FeatureCollection" },

    { "bbox", bbox_vec},

    { "features", feat_col_array },

  };

  return Geom_Coll_OBJ.dump();
}

