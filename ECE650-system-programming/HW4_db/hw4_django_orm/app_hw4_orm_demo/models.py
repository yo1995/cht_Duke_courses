from django.db import models

# Create your models here.

class Color(models.Model):
    color_id = models.AutoField(primary_key=True)
    name = models.TextField(unique=True)

    class Meta:
        managed = False
        db_table = 'color'


class Player(models.Model):
    player_id = models.AutoField(primary_key=True)
    team = models.ForeignKey('Team', models.DO_NOTHING)
    uniform_num = models.IntegerField()
    first_name = models.TextField()
    last_name = models.TextField()
    mpg = models.IntegerField()
    ppg = models.IntegerField()
    rpg = models.IntegerField()
    apg = models.IntegerField()
    spg = models.DecimalField(max_digits=4, decimal_places=1)
    bpg = models.DecimalField(max_digits=4, decimal_places=1)

    class Meta:
        managed = False
        db_table = 'player'


class State(models.Model):
    state_id = models.AutoField(primary_key=True)
    name = models.TextField(unique=True)

    class Meta:
        managed = False
        db_table = 'state'


class Team(models.Model):
    team_id = models.AutoField(primary_key=True)
    name = models.TextField(unique=True)
    state = models.ForeignKey(State, models.DO_NOTHING)
    color = models.ForeignKey(Color, models.DO_NOTHING)
    wins = models.IntegerField()
    losses = models.IntegerField()

    class Meta:
        managed = False
        db_table = 'team'

