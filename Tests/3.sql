MAKE CHART PIECHART ( SELECT * FROM(SELECT department_id, COUNT(*) c FROM employees WHERE department_id IS NOT NULL GROUP BY department_id ORDER BY c desc) WHERE rownum < 10 );

MAKE CHART BARCHART ( SELECT * FROM(SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 2 );

BEGIN
	NULL;
END;

SELECT * FROM employees;

-- PL/SQL teszt
BEGIN
	FOR i IN 1..5 LOOP
		DBMS_OUTPUT.PUT_LINE(i || '. iteráció');
	END LOOP;
END;
