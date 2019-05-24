from django.urls import path

from . import views

urlpatterns = [
	# ex: /demo/
    path('', views.index, name='index'),
    path('query1/<int:use_mpg>/<int:min_mpg>/<int:max_mpg>/<int:use_ppg>/<int:min_ppg>/<int:max_ppg>/<int:use_rpg>/<int:min_rpg>/<int:max_rpg>/<int:use_apg>/<int:min_apg>/<int:max_apg>/<int:use_spg>/<str:min_spg>/<str:max_spg>/<int:use_bpg>/<str:min_bpg>/<str:max_bpg>/', views.query1, name='query1'),
    path('query2/<str:team_color>/', views.query2, name='query2'),
    path('query3/<str:team_name>/', views.query3, name='query3'),
    path('query4/<str:team_state>/<str:team_color>/', views.query4, name='query4'),
    path('query5/<int:num_wins>/', views.query5, name='query5'),
    # # ex: /demo/5/
    # path('<int:question_id>/', views.detail, name='detail'),
    # # ex: /demo/5/results/
    # path('<int:question_id>/results/', views.results, name='results'),
    # # ex: /demo/5/vote/
    # path('<int:question_id>/vote/', views.vote, name='vote'),
]
