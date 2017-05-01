MAKE CHART PIECHART ( SELECT * FROM (SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 10 );

MAKE CHART PIECHART ( SELECT * FROM (SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 1 );

MAKE CHART PIECHART ( SELECT * FROM (SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 20 );

MAKE CHART PIECHART ( SELECT * FROM employees );

MAKE CHART BARCHART (SELECT * FROM (SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 10);

MAKE CHART BARCHART ( SELECT * FROM (SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 20 );

MAKE CHART thereisnosuchchart ( SELECT * FROM (SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 20 );