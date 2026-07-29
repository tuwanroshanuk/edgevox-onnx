#!/usr/bin/env python3
"""Export Wfloat Emotional VITS safetensors for Edgevox/Piper inference."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

import onnx
import torch


class OnnxWfloat(torch.nn.Module):
    def __init__(self, generator: torch.nn.Module) -> None:
        super().__init__()
        self.generator = generator

    def forward(
        self,
        input_ids: torch.Tensor,
        input_lengths: torch.Tensor,
        scales: torch.Tensor,
        sid: torch.Tensor,
    ) -> torch.Tensor:
        audio, *_ = self.generator.infer(
            input_ids,
            input_lengths,
            sid=sid,
            noise_scale=scales[0],
            length_scale=scales[1],
            noise_scale_w=scales[2],
        )
        return audio


def get_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--model-dir",
        type=Path,
        required=True,
        help="Wfloat Hugging Face snapshot containing model.safetensors.",
    )
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--opset", type=int, default=15)
    return parser.parse_args()


def add_metadata(path: Path, config: dict) -> None:
    model = onnx.load(path)
    metadata = {
        "model_type": "wfloat_emotional_vits",
        "comment": "piper wfloat_emotional_vits",
        "language": config["language"]["code"],
        "voice": config["espeak"]["voice"],
        "sample_rate": str(config["audio"]["sample_rate"]),
        "n_speakers": str(config["num_speakers"]),
        "add_blank": "0",
        "use_eos_bos": "0",
        "pad_id": "0",
        "bos_id": "1",
        "eos_id": "2",
        "wfloat_emotion_token_ids": "159,160,161,162,163,164,165,166",
        "wfloat_intensity_token_ids": "173,174,175,176,177,178,179,180,181,182",
    }
    del model.metadata_props[:]
    for key, value in metadata.items():
        item = model.metadata_props.add()
        item.key = key
        item.value = value
    onnx.save(model, path)


def write_tokens(path: Path, config: dict) -> None:
    token_map = config["phoneme_id_map"]
    id_to_symbol = {int(ids[0]): symbol for symbol, ids in token_map.items()}
    with path.open("w", encoding="utf-8", newline="\n") as output:
        for token_id in sorted(id_to_symbol):
            output.write(f"{id_to_symbol[token_id]} {token_id}\n")


def main() -> None:
    args = get_args()
    model_dir = args.model_dir.resolve()
    sys.path.insert(0, str(model_dir / "src"))

    from wfloat_tts.infer import WfloatGenerator  # pylint: disable=import-outside-toplevel

    with (model_dir / "config.json").open(encoding="utf-8") as config_file:
        config = json.load(config_file)

    generator = WfloatGenerator(
        checkpoint_path=model_dir / "model.safetensors",
        config_path=model_dir / "config.json",
    )
    model = OnnxWfloat(generator._model).eval()  # pylint: disable=protected-access

    input_ids = torch.tensor([[20, 18, 24, 27, 159, 178]], dtype=torch.int64)
    input_lengths = torch.tensor([input_ids.shape[1]], dtype=torch.int64)
    scales = torch.tensor([0.667, 1.0, 0.8], dtype=torch.float32)
    sid = torch.tensor([0], dtype=torch.int64)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with torch.inference_mode():
        torch.onnx.export(
            model,
            (input_ids, input_lengths, scales, sid),
            args.output,
            input_names=["input", "input_lengths", "scales", "sid"],
            output_names=["output"],
            dynamic_axes={
                "input": {1: "num_tokens"},
                "output": {2: "num_samples"},
            },
            opset_version=args.opset,
            dynamo=False,
        )

    add_metadata(args.output, config)
    write_tokens(args.output.with_name("tokens.txt"), config)
    print(args.output)


if __name__ == "__main__":
    main()
