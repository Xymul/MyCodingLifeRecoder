package io.xymul.magiccalc.math

class Angle(degrees: Int, minutes: Int, seconds: Int) {
    var degrees: Int = 0
    var minutes: Int = 0
        set(value) {
            var times = 0
            if (value >= 60) {
                times = value / 60
                field = value - times * 60
                degrees += times
            } else {
                field = value
            }
        }
    var seconds: Int = 0
        set(value) {
            var times = 0
            if (value >= 60) {
                times = value / 60
                field = value - times * 60
                minutes += times
            } else {
                field = value
            }
        }

    init {
        val totalSeconds = degrees * 3600 + minutes * 60 + seconds
        val d = totalSeconds / 3600
        this.degrees = d
        val m = (totalSeconds - d * 3600) / 60
        this.minutes = m
        this.seconds = totalSeconds - d * 3600 - m * 60
    }

    companion object {
        fun totalSeconds(angle: Angle) = angle.degrees * 3600 + angle.minutes * 60 + angle.seconds

        fun fromTotalSeconds(totalSeconds: Int): Angle {
            val degree = totalSeconds / 3600
            val minute = (totalSeconds - degree * 3600) / 60
            val second = totalSeconds - degree * 3600 - minute * 60
            return Angle(degree, minute, second)
        }
    }

    fun toRadians(): Double {
        val secondsPercent = this.seconds.toDouble() / 3600.0
        val minutesPercent = this.minutes.toDouble() / 60
        val totalDegrees = degrees + minutesPercent + secondsPercent
        return (totalDegrees / 180) * Math.PI
    }

    fun toRadiansString(): String {
        val secondsPercent = this.seconds.toDouble() / 3600.0
        val minutesPercent = this.minutes.toDouble() / 60
        val totalDegrees = degrees + minutesPercent + secondsPercent
        return "${totalDegrees}PI"
    }

    operator fun plus(angle: Angle): Angle = fromTotalSeconds(totalSeconds(this) + totalSeconds(angle))

    operator fun minus(angle: Angle): Angle = fromTotalSeconds(totalSeconds(this) - totalSeconds(angle))

    fun mulInt(times: Int) = Angle(this.degrees * times, this.minutes * times, this.seconds * times)

    fun divideInt(angle: Angle) = totalSeconds(this) / totalSeconds(angle)

    fun divideDouble(angle: Angle) = totalSeconds(this).toDouble() / totalSeconds(angle).toDouble()

    fun divideRemainder(angle: Angle): Angle {
        val times = this.divideInt(angle)
        val subObj = angle.mulInt(times)
        return this.minus(subObj)
    }

}
