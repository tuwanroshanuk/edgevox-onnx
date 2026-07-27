#!/usr/bin/env python3
# Synchronize edgevox-onnx-tts.node.js from edgevox-onnx-tts.js.
# The only difference should be the export mechanism:
#   edgevox-onnx-tts.js      uses ES module: export { ... }
#   edgevox-onnx-tts.node.js uses CommonJS:  module.exports = { ... }

import os
import re

dir_path = os.path.dirname(os.path.abspath(__file__))
src = os.path.join(dir_path, "edgevox-onnx-tts.js")
dst = os.path.join(dir_path, "edgevox-onnx-tts.node.js")

with open(src) as f:
    content = f.read()

# Replace ES module export with CommonJS module.exports
content = re.sub(
    r"export \{[^}]*\};",
    """if (typeof process == 'object' && typeof process.versions == 'object' &&\\n    typeof process.versions.node == 'string') {\\n  module.exports = {\\n    createOfflineTts,\\n    getDefaultOfflineTtsModelType,\\n  };\\n}""",
    content,
)

with open(dst, "w") as f:
    f.write(content)

print(f"Updated {dst} from {src}")
