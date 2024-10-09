package io.xymul.magiccalc

import io.xymul.magiccalc.geotools.GeoCoordinate
import io.xymul.magiccalc.geotools.GeoMap
import io.xymul.magiccalc.math.Angle

fun main() {
    val gc = GeoCoordinate(Angle(155, 22, 48), Angle(15, 0, 0), isWest = false)
    println(gc.mapSubdivisionNumber(GeoMap.Scales.TEN_THOUSAND))
}