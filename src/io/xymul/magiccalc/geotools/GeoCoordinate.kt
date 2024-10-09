package io.xymul.magiccalc.geotools

import io.xymul.magiccalc.math.Angle
// 除格点以外均能正确计算
data class GeoCoordinate(val longitude: Angle,
                         val latitude: Angle,
                         var isWest: Boolean = true,
                         var isNorth: Boolean = true) {

    fun mapSubdivisionNumber(scale: GeoMap.Scales): String {
        if (!isNorth) { return "" }

        var result = ""
        var line1 = latitude.degrees / 4
        var raw1 = longitude.degrees / 6
        // that symbolizes ${angle / 4d} have a reminder
        if (latitude.degrees % 4 != 0 || latitude.minutes != 0 || latitude.seconds != 0) {
            line1 += 1
        }
        if (longitude.degrees % 6 != 0 || longitude.minutes != 0 || longitude.seconds != 0) {
            raw1 += 1
        }
        if (!isWest) raw1 += 30
        if (line1 == 0) line1 += 1
        if (raw1 == 0) raw1 += 1
        result += "${(line1 + 64).toChar()}-$raw1"
        val latitudeReminder1 = latitude.divideRemainder(Angle(4, 0, 0))
        val longitudeReminder1 = longitude.divideRemainder(Angle(6, 0, 0))
        // A ------- B
        // |  x-y    |
        // C ------- D
        // todo:

        val haveReminder = { a: Angle ->
            a.degrees != 0 || a.seconds != 0 || a.minutes != 0
        }

        val abcd = { a: Int, b: Int ->
            if (a == 1) {
                if (b == 1) {
                    result += "-C"
                }
                if (b == 2) {
                    result += "-D"
                }
            }
            if (a == 2) {
                if (b == 1) {
                    result += "-A"
                }
                if (b == 2) {
                    result += "-B"
                }
            }
        }

        when (scale) {
            GeoMap.Scales.MILLION -> {} // 1 : 1,000,000

            GeoMap.Scales.TWENTY_FIVE_HUNDRED_THOUSAND, // 1 : 250,000 & 1 : 500,000
            GeoMap.Scales.FIVE_HUNDRED_THOUSAND -> {
                var line2 = latitudeReminder1.degrees / 2
                if (latitudeReminder1.degrees % 2 != 0 || latitudeReminder1.minutes != 0 || latitudeReminder1.seconds != 0) {
                    line2 += 1
                }
                var raw2 = longitudeReminder1.degrees / 3
                if (longitudeReminder1.degrees % 3 != 0 || latitudeReminder1.minutes != 0 || latitudeReminder1.seconds != 0) {
                    raw2 += 1
                }
                if (line2 == 0) line2 += 1
                if (raw2 == 0) raw2 += 1
                abcd(line2, raw2)

                if (scale == GeoMap.Scales.TWENTY_FIVE_HUNDRED_THOUSAND) {
                    val latitudeReminder2 = latitudeReminder1.divideRemainder(Angle(2, 0, 0))
                    val longitudeReminder2 = longitudeReminder1.divideRemainder(Angle(3, 0, 0))

                    var line3 = latitudeReminder2.divideInt(Angle(1, 0, 0))
                    var raw3 = longitudeReminder2.divideInt(Angle(1, 30, 0))
                    if (latitudeReminder2.minutes != 0 || latitudeReminder2.seconds != 0) {
                        line3 += 1
                    }

                    val longitudeReminder3 = longitudeReminder2.divideRemainder(Angle(1, 30, 0))
                    if (longitudeReminder3.degrees != 0 || longitudeReminder3.minutes != 0 || longitudeReminder3.seconds != 0) {
                        raw3 += 1
                    }
                    if (line3 == 0) line3 += 1
                    if (raw3 == 0) raw3 += 1
                    result += "-[${(4 - line3) * 4 + raw3}]"
                }
            }
            GeoMap.Scales.ONE_HUNDRED_HUNDRED,
            GeoMap.Scales.FIFTY_THOUSAND,
            GeoMap.Scales.TWENTY_FIVE_THOUSAND,
            GeoMap.Scales.TEN_THOUSAND-> {
                var line2 = latitudeReminder1.divideInt(Angle(0, 20, 0))
                var raw2 = longitudeReminder1.divideInt(Angle(0, 30, 0))
                val latitudeReminder2 = latitudeReminder1.divideRemainder(Angle(0, 20, 0))
                val longitudeReminder2 = longitudeReminder1.divideRemainder(Angle(0, 30, 0))
                if (haveReminder(latitudeReminder2)) {
                    line2 += 1
                }
                if (haveReminder(longitudeReminder2)) {
                    raw2 += 1
                }
                if (line2 == 0) line2 += 1
                if (raw2 == 0) raw2 += 1
                result += "-${(12 - line2) * 12 + raw2}"

                if (scale == GeoMap.Scales.FIFTY_THOUSAND || scale == GeoMap.Scales.TWENTY_FIVE_THOUSAND) {
                    var line3 = latitudeReminder2.divideInt(Angle(0, 10, 0))
                    var raw3 = latitudeReminder2.divideInt(Angle(0, 15, 0))
                    val latitudeReminder3 = latitudeReminder2.divideRemainder(Angle(0, 10, 0))
                    val longitudeReminder3 = longitudeReminder2.divideRemainder(Angle(0, 15, 0))
                    if (haveReminder(latitudeReminder3)) {
                        line3 += 1
                    }
                    if (haveReminder(longitudeReminder3)) {
                        raw3 += 1
                    }
                    if (line3 == 0) line3 += 1
                    if (raw3 == 0) raw3 += 1

                    abcd(line3, raw3)

                    if (scale == GeoMap.Scales.TWENTY_FIVE_THOUSAND) {
                        var line4 = latitudeReminder3.divideInt(Angle(0, 10, 0))
                        var raw4 = latitudeReminder3.divideInt(Angle(0, 15, 0))
                        val latitudeReminder4 = latitudeReminder3.divideRemainder(Angle(0, 10, 0))
                        val longitudeReminder4 = longitudeReminder3.divideRemainder(Angle(0, 15, 0))
                        if (haveReminder(latitudeReminder4)) {
                            line4 += 1
                        }
                        if (haveReminder(longitudeReminder4)) {
                            raw4 += 1
                        }
                        if (line4 == 0) line4 += 1
                        if (raw4 == 0) raw4 += 1
                        if (line4 == 1) {
                            if (raw4 == 1) {
                                result += "-3"
                            }
                            if (raw4 == 2) {
                                result += "-4"
                            }
                        }
                        if (line4 == 2) {
                            if (raw4 == 1) {
                                result += "-1"
                            }
                            if (raw4 == 2) {
                                result += "-2"
                            }
                        }
                    }
                }

                if (scale == GeoMap.Scales.TEN_THOUSAND) { // todo
                    var line3 = latitudeReminder2.divideInt(Angle(0, 2, 30))
                    var raw3 = longitudeReminder2.divideInt((Angle(0, 3, 45)))
                    val latitudeReminder3 = latitudeReminder2.divideRemainder((Angle(0, 2, 30)))
                    val longitudeReminder3 = longitudeReminder2.divideRemainder((Angle(0, 3, 45)))
                    if (haveReminder(latitudeReminder3)) {
                        line3 += 1
                    }
                    if (haveReminder(longitudeReminder3)) {
                        raw3 += 1
                    }
                    if (line3 == 0) line3 += 1
                    if (raw3 == 0) raw3 += 1
                    result += "-(${(8 - line3) * 8 + raw3})"
                }
            }
        }

        return result
    }

}
