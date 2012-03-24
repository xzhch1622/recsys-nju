begin;

drop table if exists Keyword_Item_Weight;
drop table if exists Keyword;

create table if not exists Keyword(
	id bigint(10) primary key auto_increment,
	keyword varchar(255) not null,
	count bigint(10)
);

create table if not exists Keyword_Item_Weight(
	id bigint(10) primary key auto_increment,
	keyword varchar(255) not null,
	item varchar(255) not null,
	weight float(10)
);



commit;