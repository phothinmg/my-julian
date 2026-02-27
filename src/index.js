let myJulian;

try {
  myJulian = require("../build/Release/myJulian.node");
} catch (buildError) {
  try {
    myJulian = require("../dist/myJulian.node");
  } catch (distError) {
    const error = new Error(
      "Failed to load native addon. Tried build/Release/myJulian.node and dist/myJulian.node."
    );
    error.cause = { buildError, distError };
    throw error;
  }
}

module.exports = myJulian;