select c.customer_name, c.customer_last_name, o.order_id, b.title, d.quantity, d.price, o.status, o.order_date
from 
store.customers c 
join store.orders o on c.customer_id = o.customer_id
join store.detaile_orders d on o.order_id = d.order_id
join store.books b on b.book_id = d.book_id;

create or replace view store.ship_orders_v as
select o.order_id, s.shipment_id, c.customer_name, c.customer_last_name, s.status, o.order_date, s.date_shipment, 
s.track_number, a.country || ' ' || a.city || ' ' || a.street || ' ' || a.house || ',' || a.postcode
from store.orders o 
join store.customers c on c.customer_id = o.customer_id
join store.shipments s on s.order_id = o.order_id
join store.address a on a.address_id = s.address_id;

