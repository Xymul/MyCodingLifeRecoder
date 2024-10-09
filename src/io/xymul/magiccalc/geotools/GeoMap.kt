package io.xymul.magiccalc.geotools

data class GeoMap(val fromCoord: GeoCoordinate, val toCoord: GeoCoordinate) {
    enum class Scales {
        MILLION, // 1 : 1,000,000
        FIVE_HUNDRED_THOUSAND, // 1 : 500,000
        TWENTY_FIVE_HUNDRED_THOUSAND, // 1 : 250,000
        ONE_HUNDRED_HUNDRED, // 1 : 100,000
        FIFTY_THOUSAND, // 1 : 50,000
        TWENTY_FIVE_THOUSAND, // 1 : 25,000
        TEN_THOUSAND // 1 : 10,000
    }
}
