# 10-webserver project

# How to use

use `make` command in terminal

run server using `./http_server -p 8080`



# Test Cases & Expected Results
- open `http://localhost:8080/static/images/snoopy.png` in browser  
    - snoopy image
- `curl http://localhost:8080/static/test.txt`
    - txt file contents   
- `curl http://localhost:8080/calc/add/5/7`
    - `Result: 5 + 7 = 12`
- `curl http://localhost:8080/calc/mul/6/9`
    - `Result: 6 * 9 = 54`
- `curl http://localhost:8080/calc/div/20/5`
    - `Result: 20 / 5 = 4`
- `curl http://localhost:8080/calc/div/4/0`
    - `400 Division by Zero`
- `curl http://localhost:8080/calc/mod/4/2`
    - `400 Unknown Operation`
- `curl http://localhost:8080/sleep/5`
    - sleeps 5 seconds -> `Slept for 5 seconds`
