import json
import platform
import warnings
import math

def print_tree(data):
    key_map = {}

    depth_widths = {}

    def display_key(key):
        return str(key_map.get(key, key))

    def is_scalar(value):
        return not isinstance(value, (dict, list))

    def is_scalar_list(value):
        return isinstance(value, list) and all(is_scalar(v) for v in value)

    def format_value(value):
        if isinstance(value, list):
            return "[" + ", ".join(str(v) for v in value) + "]"
        return str(value)

    def collect_widths(obj, depth=0):
        if isinstance(obj, dict):
            for key, value in obj.items():
                name = display_key(key)

                if is_scalar(value) or is_scalar_list(value):
                    depth_widths[depth] = max(
                        depth_widths.get(depth, 0),
                        len(name)
                    )

                if isinstance(value, dict):
                    collect_widths(value, depth + 1)

                elif isinstance(value, list):
                    if is_scalar_list(value):
                        continue

                    for item in value:
                        if isinstance(item, dict):
                            collect_widths(item, depth + 1)
                        elif isinstance(item, list):
                            collect_widths(item, depth + 1)

        elif isinstance(obj, list):
            if is_scalar_list(obj):
                return

            for item in obj:
                if isinstance(item, dict):
                    collect_widths(item, depth)
                elif isinstance(item, list):
                    collect_widths(item, depth)

    def print_dict(obj, prefix="", depth=0):
        items = list(obj.items())

        for i, (key, value) in enumerate(items):
            is_last = i == len(items) - 1
            branch = "└" if is_last else "├"
            child_prefix = prefix + ("   " if is_last else "│  ")
            name = display_key(key)

            if isinstance(value, dict):
                print(f"{prefix}{branch} {name}")
                print_dict(value, child_prefix, depth + 1)

            elif is_scalar_list(value):
                width = depth_widths.get(depth, len(name))
                print(f"{prefix}{branch} {name:<{width}} : {format_value(value)}")

            elif isinstance(value, list):
                print(f"{prefix}{branch} {name}")
                print_list(value, child_prefix, depth + 1)

            else:
                width = depth_widths.get(depth, len(name))
                print(f"{prefix}{branch} {name:<{width}} : {format_value(value)}")

    def print_list(obj, prefix="", depth=0):
        for i, value in enumerate(obj):
            is_last = i == len(obj) - 1
            branch = "└" if is_last else "├"
            child_prefix = prefix + ("   " if is_last else "│  ")

            if isinstance(value, dict):
                print(f"{prefix}{branch} [{i}]")
                print_dict(value, child_prefix, depth)

            elif is_scalar_list(value):
                print(f"{prefix}{branch} [{i}] : {format_value(value)}")

            elif isinstance(value, list):
                print(f"{prefix}{branch} [{i}]")
                print_list(value, child_prefix, depth)

            else:
                print(f"{prefix}{branch} [{i}] : {format_value(value)}")

    collect_widths(data)

    items = list(data.items())

    for i, (key, value) in enumerate(items):
        is_first = i == 0
        is_last = i == len(items) - 1

        if is_first:
            branch = "┌"
        elif is_last:
            branch = "└"
        else:
            branch = "├"

        child_prefix = "   " if is_last else "│  "
        name = display_key(key)

        if isinstance(value, dict):
            print(f"{branch} {name}")
            print_dict(value, child_prefix, depth=1)

        elif is_scalar_list(value):
            width = depth_widths.get(0, len(name))
            print(f"{branch} {name:<{width}} : {format_value(value)}")

        elif isinstance(value, list):
            print(f"{branch} {name}")
            print_list(value, child_prefix, depth=1)

        else:
            width = depth_widths.get(0, len(name))
            print(f"{branch} {name:<{width}} : {format_value(value)}")


with open("settings.json", "r", encoding="utf-8") as f:
    data = json.load(f)

#data = data['images'][0]

#print(json.dumps(data, indent=4, ensure_ascii=False))

print_tree(data)