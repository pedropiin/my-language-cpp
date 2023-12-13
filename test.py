import subprocess

def run_test(test_path):
    print(f"Running test for {test_path}...")
    result = subprocess.run(["powershell", "./build/compiler", test_path], capture_output=True, text=True, shell=True)

    if result.returncode == 35:
        print("Test passed.")
    else:
        print("Test failed. Error output:")
        print(result.stderr)
    
    print("Exit code:", result.returncode)


if __name__ == "__main__":
    run_test("./teste.ml")