from django.contrib import admin

# Register your models here.
from .models import Player, Team, State, Color

admin.site.register(Player)
admin.site.register(Team)
admin.site.register(State)
admin.site.register(Color)
