import sys

FIFO_PATH = "/tmp/bckl_ctrl"


try:
    with open(FIFO_PATH, 'w') as fifo_file:
        fifo_file.write(str(sys.argv[1]) + '\n')
        fifo_file.flush()

except FileNotFoundError:
    # Handle the case where the FIFO file does not exist
    print(f"Error: FIFO '{FIFO_PATH}' not found. Make sure 'mkfifo' created it before running the reader.")
except Exception as e:
    # Catch any other potential errors during file operations
    print(f"An error occurred: {e}")
