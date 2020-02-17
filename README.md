# Parallel Workloads
Optimising the arrangement of [Parallel Workloads Archive](https://www.cse.huji.ac.il/labs/parallel/workload/) logs.

## Compile
`g++ main.cpp -o a`

## Usage
`./a input_file jobs_n output_file algorithm time_limit`
- `input_file` path to the input file
- `jobs_n` number of job to process
- `output_file` output file name
- `algorithm` choose one of available algorithms `basic`, `greedy`, or `optimized`
- `time_limit` time limit for optimising the greedy solution in seconds

The `time_limit` value includes the time needed to complete the `greedy` solution, since optimizations are made on the greedy solution.

### Example usage
 `./a test.txt 5 output.txt optimized 1`