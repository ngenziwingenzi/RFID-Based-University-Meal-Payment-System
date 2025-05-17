from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
import json
from .models import Student  # make sure your model is named Student

@csrf_exempt
def admin_refill(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            uid = data.get('uid')
            amount = data.get('amount')  # Get amount sent from device

            if not uid:
                return JsonResponse({'error': 'No UID provided'}, status=400)
            if amount is None:
                return JsonResponse({'error': 'No amount provided'}, status=400)

            try:
                student = Student.objects.get(uid=uid)
                student.balance += int(amount)
                student.save()
                return JsonResponse({
                    'message': f'Balance refilled by {amount} successfully',
                    'new_balance': student.balance
                })
            except Student.DoesNotExist:
                return JsonResponse({'error': 'Card not found'}, status=404)

        except Exception as e:
            return JsonResponse({'error': str(e)}, status=500)

    return JsonResponse({'error': 'Invalid request method'}, status=405)

@csrf_exempt
def student_scan(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            uid = data.get('uid')

            if not uid:
                return JsonResponse({'error': 'No UID provided'}, status=400)

            try:
                student = Student.objects.get(uid=uid)

                if student.balance >= 500:  # meal cost
                    student.balance -= 500
                    student.save()

                    # Optional: Log the meal transaction
                    from .models import Log
                    Log.objects.create(student=student, action="meal", amount=-500)

                    return JsonResponse({
                        'message': 'Meal accepted',
                        'new_balance': student.balance
                    })
                else:
                    return JsonResponse({'message': 'Insufficient balance'}, status=200)

            except Student.DoesNotExist:
                return JsonResponse({'error': 'Card not found'}, status=404)

        except Exception as e:
            return JsonResponse({'error': str(e)}, status=500)

    return JsonResponse({'error': 'Invalid request method'}, status=405)
