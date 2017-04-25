MAKE CHART PIECHART ( SELECT * FROM(SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 2 );

MAKE CHART BARCHART ( SELECT * FROM(SELECT department_id, COUNT(*) FROM employees GROUP BY department_id) WHERE rownum < 2 );

BEGIN
	NULL;
END;
/

DECLARE
	VALAMI
BEGIN
	EZNEMJO
END;
/

SELECT * FROM employees;

-- available online in file 'sample2'
DECLARE
   CURSOR c1 is
      SELECT ename, empno, sal FROM emp
         ORDER BY sal DESC;   -- start with highest paid employee
   my_ename VARCHAR2(10);
   my_empno NUMBER(4);
   my_sal   NUMBER(7,2);
BEGIN
   OPEN c1;
   FOR i IN 1..5 LOOP
      FETCH c1 INTO my_ename, my_empno, my_sal;
      EXIT WHEN c1%NOTFOUND;  /* in case the number requested */
                              /* is more than the total       */
                              /* number of employees          */
      INSERT INTO temp VALUES (my_sal, my_empno, my_ename);
      COMMIT;
   END LOOP;
   CLOSE c1;
END;
