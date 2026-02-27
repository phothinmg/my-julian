const fs = require("fs");
const path = require("path");

const rootDir = path.resolve(__dirname, "..");
const distDir = path.join(rootDir, "dist");
const sourceJs = path.join(rootDir, "src", "index.js");
const sourceTypes = path.join(rootDir, "src", "index.d.ts");
const nativeBinary = path.join(rootDir, "build", "Release", "myJulian.node");

fs.mkdirSync(distDir, { recursive: true });
fs.copyFileSync(sourceJs, path.join(distDir, "index.js"));
fs.copyFileSync(sourceTypes, path.join(distDir, "index.d.ts"));

if (!fs.existsSync(nativeBinary)) {
  throw new Error("Missing native binary at build/Release/myJulian.node. Run npm run build first.");
}

fs.copyFileSync(nativeBinary, path.join(distDir, "myJulian.node"));