const os = require('os');

let addon = null;

const platform = os.platform() === 'win32' ? 'win' : os.platform();
const arch = os.arch();

try {
  if (arch === 'x64') {
    if (platform === 'win') {
      // @ts-expect-error
      addon = require('../edgevox-onnx-win-x64/edgevox-onnx.node')
    } else if (platform === 'darwin') {
      // @ts-expect-error
      addon = require('../edgevox-onnx-darwin-x64/edgevox-onnx.node')
    } else if (platform === 'linux') {
      // @ts-expect-error
      addon = require('../edgevox-onnx-linux-x64/edgevox-onnx.node')
    }
  } else if (arch === 'arm64') {
    if (platform === 'darwin') {
      // @ts-expect-error
      addon = require('../edgevox-onnx-darwin-arm64/edgevox-onnx.node')
    } else if (platform === 'linux') {
      // @ts-expect-error
      addon = require('../edgevox-onnx-linux-arm64/edgevox-onnx.node')
    }
  } else if (arch === 'ia32') {
    if (platform === 'win') {
      // @ts-expect-error
      addon = require('../edgevox-onnx-win-ia32/edgevox-onnx.node')
    }
  }
} catch (error) {
  //
}

if (!addon) {
  try {
    if (arch === 'x64') {
      if (platform === 'win') {
        // @ts-expect-error
        addon = require('./node_modules/edgevox-onnx-win-x64/edgevox-onnx.node')
      } else if (platform === 'darwin') {
        // @ts-expect-error
        addon = require('./node_modules/edgevox-onnx-darwin-x64/edgevox-onnx.node')
      } else if (platform === 'linux') {
        // @ts-expect-error
        addon = require('./node_modules/edgevox-onnx-linux-x64/edgevox-onnx.node')
      }
    } else if (arch === 'arm64') {
      if (platform === 'darwin') {
        // @ts-expect-error
        addon = require('./node_modules/edgevox-onnx-darwin-arm64/edgevox-onnx.node')
      } else if (platform === 'linux') {
        // @ts-expect-error
        addon = require('./node_modules/edgevox-onnx-linux-arm64/edgevox-onnx.node')
      }
    } else if (arch === 'ia32') {
      if (platform === 'win') {
        // @ts-expect-error
        addon = require('./node_modules/edgevox-onnx-win-ia32/edgevox-onnx.node')
      }
    }
  } catch (error) {
    //
  }
}
 
module.exports = addon;