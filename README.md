# NodeJs c++ addon for Julian Date

Native Node.js addon that converts Gregorian date/time to Julian Date.

## Install

```bash
npm install @phothinmaung/julian
```

The package uses `dist/myJulian.node` when available. If a compatible prebuilt binary is not present, it falls back to building from source with `node-gyp` during install.

## Usage

```js
const julian = require("@phothinmaung/julian");

const jd = julian.gregorianToJulian(2025, 2, 22);
console.log(jd);

const [year, month, day] = julian.julianToGregorian(jd);
console.log(year, month, day);
```

## API

### `gregorianToJulian(year, month, day, hour?, minutes?, seconds?, tz?)`

- `year`: number
- `month`: number (1-12)
- `day`: number
- `hour`: number, default `12`
- `minutes`: number, default `0`
- `seconds`: number, default `0`
- `tz`: timezone offset from UTC in hours, default `0`

Returns: `number` (Julian Date)

### `julianToGregorian(julianDate, tz?)`

- `julianDate`: number
- `tz`: timezone offset from UTC in hours, default `0`

Returns: `[year, month, day]` as a tuple of numbers

## Build requirements (from source)

This package contains a native `.node` binary and source files. If you need to build from source, install:

- Node.js + npm
- Python (required by `node-gyp`)
- C/C++ build tools (`make`, `g++` on Linux)

Build locally:

```bash
npm run build
```