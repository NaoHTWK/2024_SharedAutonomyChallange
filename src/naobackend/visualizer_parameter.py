from typing import Sequence


class VisualizerParameter:
    name: str
    recv_time: int
    entries: dict[str, Sequence[float | int]]

    def flatten(self) -> dict[str, any]:
        d = dict()

        for k, v in self.entries.items():
            d[f"{self.name}.{k}"] = v[0] if len(v) == 1 else v

        return d

    def __init__(self, name: str):
        self.name = name
        self.recv_time = 0
        self.entries = dict()

    def __str__(self):
        return f"VisualParamter: name={self.name} recv_time={self.recv_time} entries={str(self.entries)}"
