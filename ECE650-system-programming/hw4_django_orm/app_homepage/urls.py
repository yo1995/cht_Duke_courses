from django.urls import path

from . import views

urlpatterns = [
	# ex: /
    path('', views.index, name='index'),
]