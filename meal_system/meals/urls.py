from django.urls import path
from . import views

urlpatterns = [
    path('api/refill/', views.admin_refill, name='admin_refill'),
    path('api/scan/', views.student_scan, name='student_scan'),
]
