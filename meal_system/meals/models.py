from django.db import models

class Student(models.Model):
    uid = models.CharField(max_length=50, unique=True)
    student_id = models.CharField(max_length=20, unique=True)
    name = models.CharField(max_length=100)
    balance = models.IntegerField(default=0)

    def __str__(self):
        return f"{self.name} ({self.student_id})"

class Log(models.Model):
    student = models.ForeignKey(Student, on_delete=models.CASCADE)
    timestamp = models.DateTimeField(auto_now_add=True)
    action = models.CharField(max_length=20)  # 'meal' or 'refill'
    amount = models.IntegerField()            # e.g. -1 or +3

    def __str__(self):
        return f"{self.timestamp} - {self.student.name} - {self.action}"
