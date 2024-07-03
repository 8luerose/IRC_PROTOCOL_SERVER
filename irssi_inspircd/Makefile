NAME = irc
USER = taehkwon

# Docker Compose 파일 위치 지정
COMPOSE_SOURCE = ./docker-compose.yml

all: build up

build:
	@docker-compose -f $(COMPOSE_SOURCE) build

up:
	@docker-compose -f $(COMPOSE_SOURCE) up -d

down:
	@docker-compose -f $(COMPOSE_SOURCE) down

start:
	@docker-compose -f $(COMPOSE_SOURCE) start

stop:
	@docker-compose -f $(COMPOSE_SOURCE) stop

clean: stop
	@make down
	@if [ $$(docker ps -aq | wc -l) -gt 0 ]; then docker rm -f $$(docker ps -aq); fi
	@if [ $$(docker images -aq | wc -l) -gt 0 ]; then docker rmi -f $$(docker images -aq); fi
	@docker builder prune -f

fclean: clean
	@docker-compose -f $(COMPOSE_SOURCE) down --volumes
	@docker volume prune -f
	@docker network prune -f

ffclean: fclean
	@docker system prune -a -f

re: fclean
	@make all

.PHONY: all build up down start stop clean fclean ffclean re
