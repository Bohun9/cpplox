import glob
import re
import subprocess

for file in glob.glob("test/*.lox"):
    with open(file, "r") as stream:
        source = stream.read()

        expected_stdout = ""
        expected_stderr = ""
        for match in re.finditer(r"// out: (.*\n)", source):
            expected_stdout += match.group(1)
        for match in re.finditer(r"// err: (.*\n)", source):
            expected_stderr += match.group(1)

        result = subprocess.run(["build/lox", file], capture_output=True, text=True)

        print(f"### {file}: ", end="")
        if expected_stdout != result.stdout or expected_stderr != result.stderr:
            print("\033[1m\033[31mWA\033[m\033[0m")

            def pretty_print(header, o, e):
                print(f"\033[4m{header}:\033[0m")
                print(o, end="")
                print("\033[4mexpected:\033[0m")
                print(e, end="")

            if expected_stdout != result.stdout:
                pretty_print("standard output", result.stdout, expected_stdout)
            if expected_stderr != result.stderr:
                pretty_print("standard error", result.stderr, expected_stderr)
        else:
            print("\033[1m\033[32mOK\033[m\033[0m")
