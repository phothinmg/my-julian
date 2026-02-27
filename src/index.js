let myJulian;

try {
  myJulian = require("../dist/myJulian.node");
} catch (distError) {
  try {
    myJulian = require("../build/Release/myJulian.node");
  } catch (buildError) {
    const error = new Error(
      "Failed to load native addon. Tried dist/myJulian.node and build/Release/myJulian.node."
    );
    error.cause = { distError, buildError };
    throw error;
  }
}

module.exports = myJulian;