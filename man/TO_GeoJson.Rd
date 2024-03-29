% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/utils.R
\docType{class}
\name{TO_GeoJson}
\alias{TO_GeoJson}
\title{converts data to a GeoJson object}
\usage{
# utl <- TO_GeoJson$new()
}
\value{
a List
}
\description{
converts data to a GeoJson object

converts data to a GeoJson object
}
\section{Methods}{


\describe{
 \item{\code{TO_GeoJson$new()}}{}

 \item{\code{--------------}}{}

 \item{\code{Point(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{MultiPoint(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{LineString(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{MultiLineString(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{Polygon(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{MultiPolygon(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{GeometryCollection(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{Feature(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}

 \item{\code{FeatureCollection(data, stringify = FALSE)}}{}

 \item{\code{--------------}}{}
 }
}

\examples{

library(geojsonR)


# initialize class

init = TO_GeoJson$new()


# Examples covering all geometry-objects


# Point

point_dat = c(100, 1.01)

point = init$Point(point_dat, stringify = TRUE)
point


# MultiPoint

multi_point_dat = list(c(100, 1.01), c(200, 2.01))

multi_point = init$MultiPoint(multi_point_dat, stringify = TRUE)
multi_point


# LineString

linestring_dat = list(c(100, 1.01), c(200, 2.01))

line_string = init$LineString(linestring_dat, stringify = TRUE)
line_string


# MultiLineString

multilinestring_dat = list(list(c(100, 0.0), c(101, 1.0)), list(c(102, 2.0), c(103, 3.0)))

multiline_string = init$MultiLineString(multilinestring_dat, stringify = TRUE)
multiline_string


# Polygon (WITHOUT interior rings)

polygon_WITHOUT_dat = list(list(c(100, 1.01), c(200, 2.01), c(100, 1.0), c(100, 1.01)))

polygon_without = init$Polygon(polygon_WITHOUT_dat, stringify = TRUE)
polygon_without


# Polygon (WITH interior rings)

polygon_WITH_dat = list(list(c(100, 1.01), c(200, 2.01), c(100, 1.0), c(100, 1.01)),

                   list(c(50, 0.5), c(50, 0.8), c(50, 0.9), c(50, 0.5)))

polygon_with = init$Polygon(polygon_WITH_dat, stringify = TRUE)
polygon_with


# MultiPolygon

# the first polygon is without interior rings and the second one is with interior rings

multi_polygon_dat = list(list(list(c(102, 2.0), c(103, 2.0), c(103, 3.0), c(102, 2.0))),

                         list(list(c(100, 0.0), c(101, 1.0), c(101, 1.0), c(100, 0.0)),

                              list(c(100.2, 0.2), c(100.2, 0.8), c(100.8, 0.8), c(100.2, 0.2))))

multi_polygon = init$MultiPolygon(multi_polygon_dat, stringify = TRUE)
multi_polygon



# GeometryCollection (named list)


Point = c(100, 1.01)

MultiPoint = list(c(100, 1.01), c(200, 2.01))

MultiLineString = list(list(c(100, 0.0), c(101, 1.0)),

                  list(c(102, 2.0), c(103, 3.0)))

LineString = list(c(100, 1.01), c(200, 2.01))

MultiLineString = list(list(c(100, 0.0), c(101, 1.0)),

                  list(c(102, 2.0), c(103, 3.0)))

Polygon = list(list(c(100, 1.01), c(200, 2.01), c(100, 1.0), c(100, 1.01)))

Polygon = list(list(c(100, 1.01), c(200, 2.01), c(100, 1.0), c(100, 1.01)),

               list(c(50, 0.5), c(50, 0.8), c(50, 0.9), c(50, 0.5)))

MultiPolygon = list(list(list(c(102, 2.0), c(103, 2.0), c(103, 3.0), c(102, 2.0))),

                    list(list(c(100, 0.0), c(101, 1.0), c(101, 1.0), c(100, 0.0)),

                    list(c(100.2, 0.2), c(100.2, 0.8), c(100.8, 0.8), c(100.2, 0.2))))


geometry_collection_dat = list(Point = Point, MultiPoint = MultiPoint,

                               MultiLineString = MultiLineString, LineString = LineString,

                               MultiLineString = MultiLineString, Polygon = Polygon,

                               Polygon = Polygon, MultiPolygon = MultiPolygon)


geometry_col = init$GeometryCollection(geometry_collection_dat, stringify = TRUE)
geometry_col


# Feature (named list)


# Empty 'properties' list

feature_dat1 = list(id = 1, bbox = c(1,2,3,4), geometry = list(Point = c(100, 1.01)),

                    properties = list())


# Nested 'properties' list

feature_dat2 = list(id = "1", bbox = c(1,2,3,4), geometry = list(Point = c(100, 1.01)),

                    properties = list(prop0 = 'value0',

                                      prop1 = 0.0, vec = c(1,2,3), lst = list(a = 1, d = 2)))


feature_obj = init$Feature(feature_dat2, stringify = TRUE)
feature_obj
cat(feature_obj$json_dump)



# FeatureCollection (named list)


# takes as input the previously created 'feature_dat1', 'feature_dat2'

feature_col_dat = list(bbox = c(-10.01, -10.01, 10.01, 10.01),

                       features = list(Feature = feature_dat1, Feature = feature_dat2))
feature_col_dat


feature_collection_obj = init$FeatureCollection(feature_col_dat, stringify = TRUE)
feature_collection_obj
cat(feature_collection_obj$json_dump)

}
\section{Methods}{
\subsection{Public methods}{
\itemize{
\item \href{#method-TO_GeoJson-new}{\code{TO_GeoJson$new()}}
\item \href{#method-TO_GeoJson-Point}{\code{TO_GeoJson$Point()}}
\item \href{#method-TO_GeoJson-MultiPoint}{\code{TO_GeoJson$MultiPoint()}}
\item \href{#method-TO_GeoJson-LineString}{\code{TO_GeoJson$LineString()}}
\item \href{#method-TO_GeoJson-MultiLineString}{\code{TO_GeoJson$MultiLineString()}}
\item \href{#method-TO_GeoJson-Polygon}{\code{TO_GeoJson$Polygon()}}
\item \href{#method-TO_GeoJson-MultiPolygon}{\code{TO_GeoJson$MultiPolygon()}}
\item \href{#method-TO_GeoJson-GeometryCollection}{\code{TO_GeoJson$GeometryCollection()}}
\item \href{#method-TO_GeoJson-Feature}{\code{TO_GeoJson$Feature()}}
\item \href{#method-TO_GeoJson-FeatureCollection}{\code{TO_GeoJson$FeatureCollection()}}
\item \href{#method-TO_GeoJson-clone}{\code{TO_GeoJson$clone()}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-new"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-new}{}}}
\subsection{Method \code{new()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$new()}\if{html}{\out{</div>}}
}

}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-Point"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-Point}{}}}
\subsection{Method \code{Point()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$Point(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-MultiPoint"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-MultiPoint}{}}}
\subsection{Method \code{MultiPoint()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$MultiPoint(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-LineString"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-LineString}{}}}
\subsection{Method \code{LineString()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$LineString(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-MultiLineString"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-MultiLineString}{}}}
\subsection{Method \code{MultiLineString()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$MultiLineString(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-Polygon"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-Polygon}{}}}
\subsection{Method \code{Polygon()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$Polygon(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-MultiPolygon"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-MultiPolygon}{}}}
\subsection{Method \code{MultiPolygon()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$MultiPolygon(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-GeometryCollection"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-GeometryCollection}{}}}
\subsection{Method \code{GeometryCollection()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$GeometryCollection(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-Feature"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-Feature}{}}}
\subsection{Method \code{Feature()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$Feature(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-FeatureCollection"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-FeatureCollection}{}}}
\subsection{Method \code{FeatureCollection()}}{
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$FeatureCollection(data, stringify = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{data}}{a list specifying the geojson geometry object}

\item{\code{stringify}}{either TRUE or FALSE, specifying if the output should also include a geojson-dump (as a character string)}
}
\if{html}{\out{</div>}}
}
}
\if{html}{\out{<hr>}}
\if{html}{\out{<a id="method-TO_GeoJson-clone"></a>}}
\if{latex}{\out{\hypertarget{method-TO_GeoJson-clone}{}}}
\subsection{Method \code{clone()}}{
The objects of this class are cloneable with this method.
\subsection{Usage}{
\if{html}{\out{<div class="r">}}\preformatted{TO_GeoJson$clone(deep = FALSE)}\if{html}{\out{</div>}}
}

\subsection{Arguments}{
\if{html}{\out{<div class="arguments">}}
\describe{
\item{\code{deep}}{Whether to make a deep clone.}
}
\if{html}{\out{</div>}}
}
}
}
