create table last_month_bill as select C.customer_id, C.address, B.amount, B.package_id, B.charged_type from Bill B join Customer C where B.customer_id=C.customer_id and C.name='KANGDM' and B.date between 20200500 and 20200600;
select customer_id, address, SUM(amount) from last_month_bill;
select package_id, name, amount, package_type, charged_type, timeliness from last_month_bill B1 natural join Package;
drop table last_month_bill;