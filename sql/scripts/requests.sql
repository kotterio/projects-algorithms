--запросы 
--1. Поиск издательства, выпустившего более 5 книг 
select
  p.publishing_name,
  count(b.book_id) as book_count
from
  store.publishing as p
join
  store.books as b on p.publishing_id = b.publishing_id
group by
  p.publishing_name
having
  COUNT(b.book_id) > 0;
--2. Получение информации о заказах и доставке
select
  o.order_id,
  o.status as order_status,
  s.status as shipment_status
from
  store.orders as o
left join
  store.shipments as s on o.order_id = s.order_id;
--3. Поиск покупателей, оставивших отзывы с рейтингом выше 4
select
  c.customer_name,
  c.customer_last_name
from
  store.customers as c
join
  store.reviews as r on c.customer_id = r.customer_id
where
  r.rating > 4;

--4. Вывод всех названий книг, выпущенных издательством AST
select
  title
from
  store.books
where
  publishing_id IN (SELECT publishing_id FROM store.publishing WHERE publishing_name = 'АСТ');

--5. Топ 3 самых продаваемых книг (как основу беру количество книг в заказах)

select
  b.title,
  sum(d.quantity) as total_quantity_ordered
from
  store.books as b
join
  store.detaile_orders as d on b.book_id = d.book_id
group by
  b.title
order by
  total_quantity_ordered desc
limit 3;

--6. Сортировка клиентов по дате регистрации
select
  customer_name,
  customer_last_name,
  rank() over (order by date_register desc) as registration_rank
from
  store.customers;
--7. Скользящий средний рейтинг книг - среднее текущей и двух предыдущих
select
  b.title,
  avg(r.rating) over (order by b.book_id asc rows between 2 preceding and current row) as moving_average_rating
from
  store.books as b
join
  store.reviews as r on b.book_id = r.book_id;
--8. Поиск клиентов, которые изменили свои данные
select
  c.customer_name,
  c.customer_last_name
from
  store.customers as c
where
  exists (select 1 from store.customer_history as ch where c.customer_id = ch.customer_id);

--9. Поиск всех книг автора Умберто Эко
select
  b.title
from
  store.books as b
join
  store.book_author as ba on b.book_id = ba.book_id
join
  store.author as a on ba.author_id = a.author_id
where
  a.author_name = 'Умберто' and a.author_last_name = 'Эко';

--10. Запрос на получение клиентов, оставивших отзывы на книги 
select 
  c.customer_id, 
  c.customer_name, 
  c.customer_last_name, 
  COUNT(r.review_id) AS review_count
from
  store.customers c
join
  store.reviews r on c.customer_id = r.customer_id
group by
  c.customer_id, 
  c.customer_name, 
  c.customer_last_name
having 
  count(r.review_id) > 0
order by
  review_count desc;

--11.Запрос на получение информации о количестве, цене и общей стоимости заказанных книг в заказе
select 
  o.order_id, 
  sum(d.quantity) as total_quantity, 
  sum(d.price * d.quantity) as total_price
from
  store.orders o
join 
  store.detaile_orders d on o.order_id = d.order_id
group by
  o.order_id
order by 
  total_price desc;











