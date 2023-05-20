import glob
import re
import subprocess
import sys

if len(sys.argv) != 2:
    print(f"usage: {sys.argv[0]} <exe>")
    sys.exit(1)
exe = sys.argv[1]


def csi(s, n):
    return f"\033[{n}m{s}\033[0m"


for file in glob.glob("test/**/*.lox", recursive=True):
    with open(file, "r") as stream:
        source = stream.read()

        expected_stdout = ""
        expected_stderr = ""
        for match in re.finditer(r"// out: (.*\n)", source):
            expected_stdout += match.group(1)
        for match in re.finditer(r"// err: (.*\n)", source):
            expected_stderr += match.group(1)

        result = subprocess.run([exe, file], capture_output=True, text=True)

        print(f"### {file}: ", end="")
        if expected_stdout != result.stdout or expected_stderr != result.stderr:
            print(csi(csi("WA", 31), 1))

            def pretty_print(header, o, e):
                print(csi(f"{header}:", 4))
                print(o, end="")
                print(csi("expected:", 4))
                print(e, end="")

            if expected_stdout != result.stdout:
                pretty_print("standard output", result.stdout, expected_stdout)
            if expected_stderr != result.stderr:
                pretty_print("standard error", result.stderr, expected_stderr)
        else:
            print(csi(csi("OK", 32), 1))
