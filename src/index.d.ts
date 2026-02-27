export = MyJulian;
export as namespace myJulian;

declare namespace MyJulian {
  function gregorianToJulian(
    year: number,
    month: number,
    day: number,
    hour?: number,
    minutes?: number,
    seconds?: number,
    tz?: number,
  ): number;

  function julianToGregorian(
    julianDate: number,
    tz?: number,
  ): [year: number, month: number, day: number];
}