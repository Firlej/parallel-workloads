# Parallel Workloads
Optimising the arrangement of [Parallel Workloads Archive](https://www.cse.huji.ac.il/labs/parallel/workload/) logs.

## Compile
`g++ main.cpp -o a`

## Usage
`./a input_file jobs_n output_file time_limit`
- `input_file` path to the input file
- `jobs_n` number of job to process
- `output_file` output file name
- `time_limit` time limit for optimising the greedy solution in seconds

To use the greedy solution only use a time limit of `0`

### Example usage
 `./a test.txt 5 output.txt 5`