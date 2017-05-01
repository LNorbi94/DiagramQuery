CREATE TABLE fortestingpurposes ( elso integer, masodik varchar2(100) );

INSERT INTO fortestingpurposes VALUES ( 10, 'Almafa' );

INSERT INTO fortestingpurposes VALUES ( 30, 'Almafa' );

INSERT INTO fortestingpurposes VALUES ( 40, 'Almafa' );

SELECT * FROM fortestingpurposes;

UPDATE fortestingpurposes SET masodik = 'Körtefa' WHERE elso = 10;

SELECT * FROM fortestingpurposes;

DELETE FROM fortestingpurposes WHERE elso = 30;

SELECT * FROM fortestingpurposes;

DROP TABLE fortestingpurposes;