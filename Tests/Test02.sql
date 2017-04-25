EXPLAIN PLAN SET STATEMENT_ID = 'temp1' FOR (SELECT * FROM employees);

SELECT cardinality "Rows", 
lpad(' ',level-1)||operation||' '||options||' '||object_name "Plan" 
				FROM PLAN_TABLE 
				CONNECT BY prior id = parent_id 
				AND prior statement_id = statement_id 
				START WITH id = 0 
				AND statement_id = 'temp1' 
                ORDER BY id;

SELECT sum(darab) FROM nikovits.hivas, nikovits.kozpont, nikovits.primer
WHERE hivas.kozp_azon_hivo=kozpont.kozp_azon AND kozpont.primer=primer.korzet
AND primer.varos = 'Szentendre' 
AND datum + 1 = next_day(to_date('2012.01.31', 'yyyy.mm.dd'),'hétfõ');
