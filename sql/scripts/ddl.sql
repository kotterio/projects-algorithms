DROP TABLE IF exists store.detaile_orders;
DROP TABLE IF exists store.shipments;
DROP TABLE IF exists store.orders;
DROP TABLE IF exists store.reviews;
DROP TABLE IF exists store.customer_history;
DROP TABLE IF exists store.customers;
DROP TABLE IF exists store.book_author;
DROP TABLE IF exists store.author;
DROP TABLE IF exists store.books;
DROP TABLE IF exists store.publishing;
DROP TABLE IF exists store.address;

drop schema if exists store;

create schema store;
create table store.address (
  address_id serial not null primary key,
  country varchar(50) not null,
  city varchar(50) not null,
  street varchar(50) not null,
  house varchar(50) not null,
  postcode int not null
);

create table store.publishing(
publishing_id serial not null primary key,
publishing_name varchar(50) not null,
address_id int unique  not null,
foreign key(address_id) references store.address(address_id),
publishing_email varchar(50) not null,
publishing_phone varchar(50) not null
);

create table store.books(
book_id serial not null primary key,
title varchar(150) not null,
publishing_id int not null,
foreign key(publishing_id) references store.publishing(publishing_id),
genre varchar(50) not null,
date_publishing date not null
);

create table store.author(
  author_id serial not null primary key,
  author_name varchar(50) not null,
  author_last_name varchar(50),
  author_email varchar(50),
  author_phone varchar(50),
  biography text,
  birth_date date
);

create table store.book_author(
book_author_id serial not null primary key,
author_id int not null,
book_id int not null,
foreign key(author_id) references store.author(author_id),
foreign key(book_id) references store.books(book_id)
);

create table store.customers(
customer_id serial not null primary key,
customer_name varchar(50) not null,
customer_last_name varchar(50) not null,
customer_email varchar(150) not null,
customer_phone varchar(50) not null,
date_register timestamp default current_timestamp
);

create table store.customer_history(
customer_history_id serial not null primary key,
customer_id int,
foreign key(customer_id) references store.customers(customer_id),
customer_name varchar(50) not null,
customer_last_name varchar(50) not null,
customer_email varchar(150) not null,
customer_phone varchar(50) not null,
date_register timestamp,
date_change timestamp default current_timestamp
);

create table store.reviews(
review_id serial not null primary key,
customer_id int,
foreign key(customer_id) references store.customers(customer_id),
book_id int,
foreign key(book_id) references store.books(book_id),
rating int not null,
description text,
date_create timestamp not null
);

create table store.orders(
order_id serial not null primary key,
customer_id int,
foreign key(customer_id) references store.customers(customer_id),
order_date date not null,
status varchar(50)
);

create table store.shipments(
shipment_id serial not null primary key,
order_id int unique,
foreign key(order_id) references store.orders(order_id),
address_id int unique not null,
foreign key(address_id) references store.address(address_id),
status varchar(50),
date_shipment timestamp,
track_number int not null
);

create table store.detaile_orders(
details_order_id serial not null primary key,
order_id int not null,
foreign key(order_id) references store.orders(order_id),
book_id int not null,
foreign key(book_id) references store.books(book_id),
quantity int not null check(quantity >= 0),
price numeric(10,2) not null check (price >= 0)
);